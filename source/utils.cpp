#include "main.h"

// -----------------------------------------------------------------------------------
// Utility function for various calculations
// -----------------------------------------------------------------------------------
double NumToPercent(double number, double ref_number) {
	return(number * 100.0 / ref_number);
}
double PercentToNum(double percent, double ref_number) {
	return(percent * ref_number / 100.0);
}
double CalcXW(double xw, double ref_width, double display_width) {
	return PercentToNum(NumToPercent(xw, ref_width), display_width);
}
double CalcYH(double yh, double ref_height, double display_height) {
	return PercentToNum(NumToPercent(yh, ref_height), display_height);
}
double DbgFontX(double x, double display_width) {	
	return NumToPercent(x, display_width) / 100.0;
}
double DbgFontY(double y, double display_height) {	
	return NumToPercent(y, display_height) / 100.0;
}
double GetCenterX(double width, double max_width) {
	return((max_width - width) / 2.0);
}
double DbgFontCenterX(double left_x, double right_x, double fontsize, unsigned int len, double ref_width, double /*ref_height*/, double display_width, double /*display_height*/)
{
	double nPointX		= CalcXW(left_x	, ref_width, display_width);
	double nMaxWidth	= CalcXW(right_x, ref_width, display_width);
	double nTxtWidth	= CalcXW(fontsize * (double)len, ref_width, display_width);
	double nCentered	= GetCenterX(nTxtWidth, nMaxWidth);
	return DbgFontX(nPointX + nCentered, display_width);
}
// -----------------------------------------------------------------------------------

// Was fun to code this xD...
void GetDateTimeStr(char* szDate, char* szTime, bool b24hour)
{
	char month[12][16] = { 
		"January", 
		"February", 
		"March", 
		"April", 
		"May", 
		"June", 
		"July", 
		"August", 
		"September", 
		"October", 
		"November", 
		"December"
	};

	char day[7][16] = {
		"Sunday", 
		"Monday", 
		"Tuesday", 
		"Wednesday", 
		"Thursday", 
		"Friday", 
		"Saturday"
	};
	
	char szAMPM[16];
	memset(&szAMPM, 0, sizeof(szAMPM));

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);	
		
	int nHour = tm.tm_hour;

	// change 24 hr format to 12 hr
	if(!b24hour) 
	{
		if(nHour > 12) {
			nHour = 12 - (24 - nHour);
		}
		
		if(nHour == 0) 
			nHour = 12;

		if(tm.tm_hour >= 12) {
			strcpy(szAMPM, " PM"); 
		} else {
			strcpy(szAMPM, " AM");
		}
	}

	sprintf(szDate, "%s, %s %d, %04d", day[tm.tm_wday], month[tm.tm_mon], tm.tm_mday, tm.tm_year + 1900);

	(nHour < 10) ? 
		sprintf(szTime, " %d:%02d:%02d%s", nHour, tm.tm_min, tm.tm_sec, szAMPM) : 
		sprintf(szTime, "%d:%02d:%02d%s", nHour, tm.tm_min, tm.tm_sec, szAMPM);

}

bool DirExist(char* szDir)
{
	struct stat st;

	if(stat(szDir,&st) == 0 && ((st.st_mode & S_IFMT) == S_IFDIR)) {
		return true;
	}
	return false;
}

bool FileExist(char* szFile)
{
	struct stat st;
	
	if(stat(szFile,&st) == 0 && ((st.st_mode & S_IFMT) == S_IFREG)) {
		return true;
	}
	return false;
}

uint64_t GetFileSize(char* szFilePath)
{
	struct stat st;
	memset(&st, 0, sizeof(struct stat));
	stat(szFilePath, &st);
	return st.st_size;
}

void init_log()
{
	fp_log = fopen("/dev_hdd0/game/SISO00123/USRDIR/log.txt", "w");
	if(fp_log) {
		fclose(fp_log);
		*&fp_log = NULL;
	}
}
