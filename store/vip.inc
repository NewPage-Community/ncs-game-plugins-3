#define VIPMonthPrice       1000
#define VIPSeasonPrice      2700
#define VIPSemiannualPrice  5280
#define VIPAnnualPrice      10000

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