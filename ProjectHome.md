When I was trying to convert Quicken for Macintosh data for use with Quicken for Windows, the export file contained only 2-digit years (1/5/08).  Quicken for Windows interpreted this as 1/5/1908, despite my best attempts to follow the "regional setting" directions.

Fortunately, the .qif file is not encrypted nor does it have a checksum, and all the date lines appeared to be of this form:
D1/5/08

So, this program will read in a file, look for a line starting with 'D' and containing two '/' characters, then insert "19" or "20" depending on the next digit.  This worked for me; your mileage may vary.

Update (2/25/2014): I had a user submit a qif file with \n instead of \r, so I updated the converter to accept either; I also changed the century breakpoint: it used to take only 90-99 as being in the 1900s, but now anything from 50-99 is assumed to be 1950-1999.  00-49 are assumed 2000-2049.  I checked in an updated File.c, but I can't figure out how to post a new binary on the Downloads tab of Google Project Hosting.  So, you'll have to click the Source tab browse to the https://code.google.com/p/qif-fix/source/browse/#svn%2Ftrunk%2Fqif-fix%2Fbinaries svn/trunk/qif-fix/binaries folder.