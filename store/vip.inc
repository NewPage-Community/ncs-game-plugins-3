#define VIPMonthPrice	   2000
#define VIPSeasonPrice	  5400
#define VIPSemiannualPrice  10560
#define VIPAnnualPrice	  20000

// Display price
int VIPPrice(int month)
{
	int price;
	switch (month)
	{
		case 1: price = VIPMonthPrice
		case 3: price = VIPSeasonPrice
		case 6: price = VIPSemiannualPrice
		case 12: price = VIPAnnualPrice
		case 24: price = VIPAnnualPrice * 2
		case 36: price = VIPAnnualPrice * 3
		default: price = VIPMonthPrice * month
	}
	return price;
}