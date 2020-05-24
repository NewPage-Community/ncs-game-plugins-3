# Makefile for the Sourcemod plugin
# NewPage Dev Team

SOURCEMOD_VERSION ?= 1.10
SOURCEMOD_BUILD_DIR = ./addons/sourcemod/scripting

.PHONY:all
all: env build

.PHONY:env
env: 
	@echo "\nSetting sourcemod $(SOURCEMOD_VERSION) environment..."
	@wget "http://www.sourcemod.net/latest.php?version=$(SOURCEMOD_VERSION)&os=linux" -q -O sourcemod.tar.gz
	@tar -xzf sourcemod.tar.gz
	@cp -rf $(SOURCEMOD_BUILD_DIR)/include ./ && cp -f $(SOURCEMOD_BUILD_DIR)/spcomp ./ && cp -f $(SOURCEMOD_BUILD_DIR)/compile.sh ./ && chmod +x spcomp

.PHONY:build
build:
	@COUNT=$(git rev-list --all --count)
	@sed -i "s%<commit_count>%$$COUNT%g" include/ncs.inc
	@test -e compiled || mkdir compiled
	@for sourcefile in *.sp; \
	do \
		smxfile="`echo $$sourcefile | sed -e 's/\.sp$$/\.smx/'`"; \
		echo "\nCompiling $$sourcefile ..."; \
		./spcomp -E $$sourcefile -ocompiled/$$smxfile; \
		if [ $$? -ne 0 ]; \
		then \
			exit 1; \
		fi \
	done