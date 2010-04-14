
/* This file is created by Panda.Xiong, 2009.04.09 */


macro Add_Modify_Information()
{
    hwnd    = GetCurrentWnd()
    hbuf    = GetCurrentBuf()

    MyName  = GetEnv(USERNAME)

    SysTime = GetSysTime(0)
    year    = SysTime.Year
    month   = SysTime.Month
    day     = SysTime.Day
    hour    = SysTime.Hour + 8
    minute  = SysTime.Minute

	lnFirst = GetWndSelLnFirst(hwnd)
	lnLast  = GetWndSelLnLast(hwnd)

    firstBuf = ""
    lastBuf  = ""
    spaceBuf = ""
    referencedBuf = GetBufLine( hbuf, lnFirst )

    i = 0
    while (referencedBuf[i] == " " || referencedBuf[i] == "\t")
    {
	    if (referencedBuf[i] == " ")
	    {
		    spaceBuf = cat( spaceBuf, " " )//space
	    }
	    else
	    {
		    spaceBuf = cat( spaceBuf, "\t" )//Tab
	    }

	    i = i + 1
    }

    firstBuf = cat( spaceBuf, "/* Modified by @MyName@, @year@/@month@/@day@ */" )
    lastBuf  = cat( spaceBuf, "/* End of Modified by @MyName@, @year@/@month@/@day@ */" )
    InsBufLine( hbuf, lnFirst, firstBuf )
	InsBufLine( hbuf, lnLast + 2, lastBuf )
}


/*-------------------------------------------------------------------------
	I N S E R T   H E A D E R

	Inserts a comment header block at the top of the current function.
	This actually works on any type of symbol, not just functions.

	To use this, define an environment variable "MYNAME" and set it
	to your email name.  eg. set MYNAME=raygr
-------------------------------------------------------------------------*/
macro Add_Function_Header()
{
	hwnd = GetCurrentWnd()
	hbuf = GetCurrentBuf()

	/* get current user name */
	szMyName = GetEnv(USERNAME)

	/* get function name and file name */

	lnFirst = GetWndSelLnFirst(hwnd)
	lnLast = GetWndSelLnLast(hwnd)

	fFuncName = FALSE
	curLine = lnFirst
	nPara = 0
	while (curLine <= lnLast)
	{
		szOringnal = GetBufLine (hbuf, curLine)
		szOringnal = cat (szOringnal, " ")
		szSize = GetBufLineLength(hbuf, curLine) + 1
		keyWord=""
		i = 0;
		nKeyBegin = 0
		nKeyNum = 0
		while(i < szSize)
		{
			if (szOringnal[i] != " " && szOringnal[i] != "*" && szOringnal[i] != "(" && szOringnal[i] != ")" && szOringnal[i] != "," && szOringnal[i] != "\t")
			{
				i = i + 1
				continue
			}
			/* the current char is " ", we have all information of last keyword. */

			if (nKeyBegin == i)
			{
				/* the first char of szOringnal is " " or there are multiple " "
					between 2 Key Word,  ignore it.
				*/
				i = i + 1
				nKeyBegin = i
				continue
			}

			/* Get KeyWord, it is depened on C/C++ language. */
			keyWord = StrMid(szOringnal, nKeyBegin, i)
			if ((keyWord == "static") || (keyWord == "const") || (keyWord == "unsigned") || (keyWord == "signed") || (keyWord == "macro") || (keyWord == "struct") || (keyWord == "IN") || (keyWord == "OUT"))
			{
				i = i + 1
				nKeyBegin = i
				continue
			}

			if (!fFuncName)
			{
				if (nKeyNum == 1)
				{
					fFuncName = TRUE
					szFunctionName = keyWord
					nKeyNum = 0
				}
				else
				{
					nKeyNum = 1
				}

				i = i + 1
				nKeyBegin = i
				continue
			}

			if (nKeyNum == 1)
			{
				nKeyNum = 0

				if (nPara == 9)
				{
					nPara = 10
					szPara10 = keyWord
				}
				if (nPara == 8)
				{
					nPara = 9
					szPara9 = keyWord
				}
				if (nPara == 7)
				{
					nPara = 8
					szPara8 = keyWord
				}
				if (nPara == 6)
				{
					nPara = 7
					szPara7 = keyWord
				}
				if (nPara == 5)
				{
					nPara = 6
					szPara6 = keyWord
				}
				if (nPara == 4)
				{
					nPara = 5
					szPara5 = keyWord
				}
				if (nPara == 3)
				{
					nPara = 4
					szPara4 = keyWord
				}
				if (nPara == 2)
				{
					nPara = 3
					szPara3 = keyWord
				}
				if (nPara == 1)
				{
					nPara = 2
					szPara2 = keyWord
				}
				if (nPara == 0)
				{
					nPara = 1
					szPara1 = keyWord
				}
			}
			else
			{
				nKeyNum = 1
			}

			i = i + 1
			nKeyBegin = i
		} /* end of while() */

		/* for test. */
		//InsBufLine( hbuf, curLine + 20, "/* curLine = @curLine@, @szOringnal@, @keyWord@ */")
		curLine = curLine + 1
	}

	InsBufLine( hbuf, lnFirst, "/******************************************************************************")
	/* Function Name */
	InsBufLine( hbuf, lnFirst + 1, " * FUNCTION NAME:")
	InsBufLine( hbuf, lnFirst + 2, " *      @szFunctionName@")

	/* Description */
	InsBufLine( hbuf, lnFirst + 3, " * DESCRIPTION:")
	InsBufLine( hbuf, lnFirst + 4, " *      N/A")

	para_format_len = 10
	InsBufLine( hbuf, lnFirst + 5, " * PARAMETERS:")
	if (nPara >= 0)
	{
		if (nPara >= 1)
		{
			para_len     = strlen(szPara1)
			para_ext_len = para_format_len - para_len
			para_buf     = " *      @szPara1@"
			while (para_ext_len > 0)
			{
				para_buf = cat(para_buf, " ");
				para_ext_len = para_ext_len - 1
			}

			InsBufLine( hbuf, lnFirst + 6, "@para_buf@: ")
		}
		else
		{
			InsBufLine( hbuf, lnFirst + 6, " *      N/A")
			nPara = 1
		}
	}
	if (nPara >= 2)
	{
		para_len     = strlen(szPara2)
		para_ext_len = para_format_len - para_len
		para_buf     = " *      @szPara2@"
		while (para_ext_len > 0)
		{
			para_buf = cat(para_buf, " ");
			para_ext_len = para_ext_len - 1
		}

		InsBufLine( hbuf, lnFirst + 7, "@para_buf@: ")
	}
	if (nPara >= 3)
	{
		para_len     = strlen(szPara3)
		para_ext_len = para_format_len - para_len
		para_buf     = " *      @szPara3@"
		while (para_ext_len > 0)
		{
			para_buf = cat(para_buf, " ");
			para_ext_len = para_ext_len - 1
		}

		InsBufLine( hbuf, lnFirst + 8, "@para_buf@: ")
	}
	if (nPara >= 4)
	{
		para_len     = strlen(szPara4)
		para_ext_len = para_format_len - para_len
		para_buf     = " *      @szPara4@"
		while (para_ext_len > 0)
		{
			para_buf = cat(para_buf, " ");
			para_ext_len = para_ext_len - 1
		}

		InsBufLine( hbuf, lnFirst + 9, "@para_buf@: ")
	}
	if (nPara >= 5)
	{
		para_len     = strlen(szPara5)
		para_ext_len = para_format_len - para_len
		para_buf     = " *      @szPara5@"
		while (para_ext_len > 0)
		{
			para_buf = cat(para_buf, " ");
			para_ext_len = para_ext_len - 1
		}

		InsBufLine( hbuf, lnFirst + 10, "@para_buf@: ")
	}
	if (nPara >= 6)
	{
		para_len     = strlen(szPara6)
		para_ext_len = para_format_len - para_len
		para_buf     = " *      @szPara6@"
		while (para_ext_len > 0)
		{
			para_buf = cat(para_buf, " ");
			para_ext_len = para_ext_len - 1
		}

		InsBufLine( hbuf, lnFirst + 11, "@para_buf@: ")
	}
	if (nPara >= 7)
	{
		para_len     = strlen(szPara7)
		para_ext_len = para_format_len - para_len
		para_buf     = " *      @szPara7@"
		while (para_ext_len > 0)
		{
			para_buf = cat(para_buf, " ");
			para_ext_len = para_ext_len - 1
		}

		InsBufLine( hbuf, lnFirst + 12, "@para_buf@: ")
	}
	if (nPara >= 8)
	{
		para_len     = strlen(szPara8)
		para_ext_len = para_format_len - para_len
		para_buf     = " *      @szPara8@"
		while (para_ext_len > 0)
		{
			para_buf = cat(para_buf, " ");
			para_ext_len = para_ext_len - 1
		}

		InsBufLine( hbuf, lnFirst + 13, "@para_buf@: ")
	}
	if (nPara >= 9)
	{
		para_len     = strlen(szPara9)
		para_ext_len = para_format_len - para_len
		para_buf     = " *      @szPara9@"
		while (para_ext_len > 0)
		{
			para_buf = cat(para_buf, " ");
			para_ext_len = para_ext_len - 1
		}

		InsBufLine( hbuf, lnFirst + 13, "@para_buf@: ")
	}
	if (nPara >= 10)
	{
		para_len     = strlen(szPara10)
		para_ext_len = para_format_len - para_len
		para_buf     = " *      @szPara10@"
		while (para_ext_len > 0)
		{
			para_buf = cat(para_buf, " ");
			para_ext_len = para_ext_len - 1
		}

		InsBufLine( hbuf, lnFirst + 14, "@para_buf@: ")
	}

	/* Return */
	InsBufLine( hbuf, lnFirst + nPara + 6, " * RETURN:")
	InsBufLine( hbuf, lnFirst + nPara + 7, " *      N/A")

	/* Notes */
	InsBufLine( hbuf, lnFirst + nPara + 8, " * NOTES:")
	InsBufLine( hbuf, lnFirst + nPara + 9, " *      N/A")

	/* History */
	SysTime = GetSysTime( 0 )
	year  = SysTime.Year
    month = SysTime.Month
    day   = SysTime.Day
    hour  = SysTime.Hour + 8
    minute= SysTime.Minute
	InsBufLine( hbuf, lnFirst + nPara + 10, " * HISTORY:")
	InsBufLine( hbuf, lnFirst + nPara + 11, " *      @year@.@month@.@day@        @szMyName@         Create/Update" )

	InsBufLine( hbuf, lnFirst + nPara + 12, " *****************************************************************************/")
}



/* InsertFileHeader:

   Inserts a comment header block at the top of the current function.
   This actually works on any type of symbol, not just functions.

   To use this, define an environment variable "MYNAME" and set it
   to your email name.  eg. set MYNAME=raygr
*/

macro Add_incFile_Header()
{
	hbuf = GetCurrentBuf()
	currWinHandler = GetCurrentWnd()

	/* get current user name */
	szMyName = GetEnv(USERNAME)

    SysTime = GetSysTime(0)
    year    = SysTime.Year
    month   = SysTime.Month
    day     = SysTime.Day

	/* get current file name */
	currFileNameWithDir = GetBufName(hbuf)
	totalFileLen = strlen(currFileNameWithDir)
	loop = totalFileLen
	while (1)
	{
		if (currFileNameWithDir[loop] == "\\")
		{
			break
		}

		loop = loop - 1
		if (loop == 0)
		{
			break
		}
	}

	if (currFileNameWithDir[loop] == "\\")
	{
		loop = loop + 1
	}

	currFileName = ""
	fileMacro    = "__"
	while (1)
	{
		currFileName = cat(currFileName, currFileNameWithDir[loop])

		if (currFileNameWithDir[loop] == ".")
		{
			fileMacro = cat(fileMacro, "_")
		}
		else
		{
			upper_val = toupper(currFileNameWithDir[loop])
			fileMacro = cat(fileMacro, upper_val)
		}

		loop = loop + 1
		if (loop == totalFileLen)
		{
			break
		}
	}

	line_no = 0
	InsBufLine(hbuf, line_no, "/******************************************************************************" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * COPYRIGHT:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   Copyright (c)  2005-2010   Source Photonics Inc.    All rights reserved." )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   This is unpublished proprietary source code of Source Photonics Inc." )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   The copyright notice above does not evidence any actual or intended" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   publication of such source code." )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * FILE NAME:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   @currFileName@" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * NODULE NAME:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   N/A" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * DESCRIPTION:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   N/A" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * HISTORY:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   @year@.@month@.@day@        @szMyName@         Create/Update" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, "*****************************************************************************/" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, "" )
	line_no = line_no + 1

	InsBufLine(hbuf, line_no, "#ifndef @fileMacro@" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, "#define @fileMacro@" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, "" )
	line_no = line_no + 1


	/* add #endif flag first */
	end_line_no = GetBufLineCount(hbuf)
	end_line_no = end_line_no
	InsBufLine(hbuf, end_line_no, "" )
	end_line_no = end_line_no + 1
	InsBufLine(hbuf, end_line_no, "#endif /* @fileMacro@ */" )
	end_line_no = end_line_no + 1
	InsBufLine(hbuf, end_line_no, "" )
}



macro Add_srcFile_Header()
{
	hbuf = GetCurrentBuf()
	currWinHandler = GetCurrentWnd()

	/* get current user name */
	szMyName = GetEnv(USERNAME)

    SysTime = GetSysTime(0)
    year    = SysTime.Year
    month   = SysTime.Month
    day     = SysTime.Day

	/* get current file name */
	currFileNameWithDir = GetBufName(hbuf)
	totalFileLen = strlen(currFileNameWithDir)
	loop = totalFileLen
	while (1)
	{
		if (currFileNameWithDir[loop] == "\\")
		{
			break
		}

		loop = loop - 1
		if (loop == 0)
		{
			break
		}
	}

	if (currFileNameWithDir[loop] == "\\")
	{
		loop = loop + 1
	}

	currFileName = ""
	fileMacro    = "__"
	while (1)
	{
		currFileName = cat(currFileName, currFileNameWithDir[loop])

		if (currFileNameWithDir[loop] == ".")
		{
			fileMacro = cat(fileMacro, "_")
		}
		else
		{
			upper_val = toupper(currFileNameWithDir[loop])
			fileMacro = cat(fileMacro, upper_val)
		}

		loop = loop + 1
		if (loop == totalFileLen)
		{
			break
		}
	}

	line_no = 0
	InsBufLine(hbuf, line_no, "/******************************************************************************" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * COPYRIGHT:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   Copyright (c)  2005-2010   Source Photonics Inc.    All rights reserved." )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   This is unpublished proprietary source code of Source Photonics Inc." )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   The copyright notice above does not evidence any actual or intended" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   publication of such source code." )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * FILE NAME:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   @currFileName@" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * NODULE NAME:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   N/A" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * DESCRIPTION:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   N/A" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * HISTORY:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   @year@.@month@.@day@        @szMyName@         Create/Update" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, "*****************************************************************************/" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, "" )
	line_no = line_no + 1
}



/* InsertFileHeader:

   Inserts a comment header block at the top of the current function.
   This actually works on any type of symbol, not just functions.

   To use this, define an environment variable "MYNAME" and set it
   to your email name.  eg. set MYNAME=raygr
*/
macro Add_IncFile_GPL_Header()
{
	hbuf = GetCurrentBuf()
	currWinHandler = GetCurrentWnd()

	/* get current user name */
	szMyName = GetEnv(USERNAME)

    SysTime = GetSysTime(0)
    year    = SysTime.Year
    month   = SysTime.Month
    day     = SysTime.Day

	/* get current file name */
	currFileNameWithDir = GetBufName(hbuf)
	totalFileLen = strlen(currFileNameWithDir)
	loop = totalFileLen
	while (1)
	{
		if (currFileNameWithDir[loop] == "\\")
		{
			break
		}

		loop = loop - 1
		if (loop == 0)
		{
			break
		}
	}

	if (currFileNameWithDir[loop] == "\\")
	{
		loop = loop + 1
	}

	currFileName = ""
	fileMacro    = "__"
	while (1)
	{
		currFileName = cat(currFileName, currFileNameWithDir[loop])

		if (currFileNameWithDir[loop] == ".")
		{
			fileMacro = cat(fileMacro, "_")
		}
		else
		{
			upper_val = toupper(currFileNameWithDir[loop])
			fileMacro = cat(fileMacro, upper_val)
		}

		loop = loop + 1
		if (loop == totalFileLen)
		{
			break
		}
	}

	line_no = 0
	InsBufLine(hbuf, line_no, "/******************************************************************************" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * (C) Copyright 2007-2010" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   This program is free software; you can redistribute it and/or" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   modify it under the terms of the GNU General Public License as" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   published by the Free Software Foundation; either version 2 of" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   the License, or (at your option) any later version." )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   This program is distributed in the hope that it will be useful," )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   but WITHOUT ANY WARRANTY; without even the implied warranty of" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   GNU General Public License for more details." )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   You should have received a copy of the GNU General Public License" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   along with this program; if not, write to the Free Software" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   Foundation, Inc., 59 Temple Place, Suite 330, Boston," )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   MA 02111-1307 USA" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * FILE NAME:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   @currFileName@" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * NODULE NAME:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   N/A" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * DESCRIPTION:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   N/A" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * HISTORY:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   @year@.@month@.@day@        @szMyName@         Create/Update" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, "*****************************************************************************/" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, "" )
	line_no = line_no + 1

	InsBufLine(hbuf, line_no, "#ifndef @fileMacro@" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, "#define @fileMacro@" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, "" )
	line_no = line_no + 1


	/* add #endif flag first */
	end_line_no = GetBufLineCount(hbuf)
	end_line_no = end_line_no
	InsBufLine(hbuf, end_line_no, "" )
	end_line_no = end_line_no + 1
	InsBufLine(hbuf, end_line_no, "#endif /* @fileMacro@ */" )
	end_line_no = end_line_no + 1
	InsBufLine(hbuf, end_line_no, "" )
}



macro Add_srcFile_GPL_Header()
{
	hbuf = GetCurrentBuf()
	currWinHandler = GetCurrentWnd()

	/* get current user name */
	szMyName = GetEnv(USERNAME)

    SysTime = GetSysTime(0)
    year    = SysTime.Year
    month   = SysTime.Month
    day     = SysTime.Day

	/* get current file name */
	currFileNameWithDir = GetBufName(hbuf)
	totalFileLen = strlen(currFileNameWithDir)
	loop = totalFileLen
	while (1)
	{
		if (currFileNameWithDir[loop] == "\\")
		{
			break
		}

		loop = loop - 1
		if (loop == 0)
		{
			break
		}
	}

	if (currFileNameWithDir[loop] == "\\")
	{
		loop = loop + 1
	}

	currFileName = ""
	fileMacro    = "__"
	while (1)
	{
		currFileName = cat(currFileName, currFileNameWithDir[loop])

		if (currFileNameWithDir[loop] == ".")
		{
			fileMacro = cat(fileMacro, "_")
		}
		else
		{
			upper_val = toupper(currFileNameWithDir[loop])
			fileMacro = cat(fileMacro, upper_val)
		}

		loop = loop + 1
		if (loop == totalFileLen)
		{
			break
		}
	}

	line_no = 0
	InsBufLine(hbuf, line_no, "/******************************************************************************" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * (C) Copyright 2007-2010" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   This program is free software; you can redistribute it and/or" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   modify it under the terms of the GNU General Public License as" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   published by the Free Software Foundation; either version 2 of" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   the License, or (at your option) any later version." )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   This program is distributed in the hope that it will be useful," )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   but WITHOUT ANY WARRANTY; without even the implied warranty of" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   GNU General Public License for more details." )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   You should have received a copy of the GNU General Public License" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   along with this program; if not, write to the Free Software" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   Foundation, Inc., 59 Temple Place, Suite 330, Boston," )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   MA 02111-1307 USA" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * FILE NAME:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   @currFileName@" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * NODULE NAME:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   N/A" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * DESCRIPTION:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   N/A" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " * HISTORY:" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *   @year@.@month@.@day@        @szMyName@         Create/Update" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, " *" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, "*****************************************************************************/" )
	line_no = line_no + 1
	InsBufLine(hbuf, line_no, "" )
	line_no = line_no + 1
}

