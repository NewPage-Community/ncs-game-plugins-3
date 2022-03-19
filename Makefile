# Makefile for the Sourcemod plugin
# NewPage Dev Team

SHELL:=/bin/bash
SOURCEMOD_VERSION ?= 1.10
SOURCEMOD_BUILD_DIR = ./addons/sourcemod/scripting

# This command is under a Creative Commons Zero license, Enjoy!                                                                                                           
# Written by Nios34<nios34@foxmail.com> using GNU Emacs. Feel free to delete it.
SOURCEMOD_BUILD_PATH = $(shell curl -IsS 'https://www.sourcemod.net/latest.php?version=$(SOURCEMOD_VERSION)&os=linux' | grep -oP '$(SOURCEMOD_VERSION)/sourcemod-.*')
SOURCEMOD_DOWNLOAD_URL = http://nexus3.nexus3:8081/repository/sourcemod/

.PHONY:all
all: env build

.PHONY:env
env: 
	@printf "\nSetting sourcemod $(SOURCEMOD_VERSION) environment..."
	@curl -sS --output sourcemod.tar.gz "$(SOURCEMOD_DOWNLOAD_URL)$(SOURCEMOD_BUILD_PATH)"
	@tar -xzf sourcemod.tar.gz
	@cp -rf $(SOURCEMOD_BUILD_DIR)/include ./ && cp -f $(SOURCEMOD_BUILD_DIR)/spcomp ./ && chmod +x spcomp

.PHONY:build
build:
ifdef CI
	@printf "\nPipeline id $(CI_PIPELINE_IID)"
	@sed -i "s%<pipeline_iid>%$(CI_PIPELINE_IID)%g" include/ncs.inc
endif
	@test -e compiled || mkdir compiled
	@test -e compiled/newpage || mkdir compiled/newpage
	@test -e compiled/stats || mkdir compiled/stats
	@test -e compiled/test || mkdir compiled/test
	@for sourcefile in *.sp; \
		do \
			smxfile="`echo $$sourcefile | sed -e 's/\.sp$$/\.smx/'`"; \
			printf "\nCompiling $$sourcefile ...\n"; \
			if [[ "$$smxfile" =~ "np-" ]]; \
			then \
				smxfile="newpage/$$smxfile"; \
			elif [[ "$$smxfile" =~ "stats-" ]]; \
			then \
				smxfile="stats/$$smxfile"; \
			elif [[ "$$smxfile" =~ "test-" ]]; \
			then \
				smxfile="test/$$smxfile"; \
			fi; \
			./spcomp -t0 -E $$sourcefile -ocompiled/$$smxfile; \
			if [[ $$? -ne 0 ]]; \
			then \
				exit 1; \
			fi; \
		done