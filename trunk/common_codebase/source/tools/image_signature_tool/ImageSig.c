/******************************************************************************
 *
 * (C) Copyright 2007-2015
 *    Panda Xiong, yaxi1984@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * History:
 *    2007.11.04    Panda Xiong       Create
 *
******************************************************************************/


#include <ctype.h>
#include <stdio.h>

typedef unsigned char       GT_UI8;
typedef signed char         GT_I8;
typedef unsigned short      GT_UI16;
typedef signed short        GT_I16;
typedef unsigned long       GT_UI32;
typedef signed long         GT_I32;

typedef enum
{
    GT_FALSE = 0,
    GT_TRUE  = 1
} GT_BOOL;

#define IN     /* input parameter            */
#define OUT    /* output parameter           */
#define INOUT  /* input and output parameter */

typedef struct
{
    /* image signature: bootloader, kernel image, or others */
    GT_UI32      signature;

    /* product code: to distinguish this product with others */
    GT_UI32      product_code;

    /* image size, or image compress size if compressed,
     * including this image header */
    GT_UI32      size;

    /* compress method: =1, LZMA; or others */
    GT_UI16      compress_method;

    /* checksum of image, including this image header */
    GT_UI16      checksum;

    /* image version, must end with character '\0' */
    GT_UI8       version[16];

    /* image description, must end with character '\0' */
    GT_UI8       description[32];

} GT_IMAGE_HEADER;


typedef enum
{
    ID_SIGNATURE = 0,
    ID_PRODUCT_CODE,
    ID_COMPRESS_METHOD,
    ID_VERSION,
    ID_DESCRIPTION,
	ID_SIGNATURE_OFFSET,
    ID_MAX
} GT_ID;

typedef struct
{
    GT_ID     id;
    GT_UI8   *keyword;
    GT_BOOL   valid;     /* when this item is searched, set it to GT_TRUE */
} GT_SEARCH_ITEM;

/* here, we use two steps to get the defined keywords:
 *  1. search "#" keyword to get the "#define" line;
 *  2. search the signature keyword to get the real data we need.
 */
#define SEARCH_KEYWORD_1ST         "#"
#define SEARCH_KEYWORD_2ND         "define"
static  GT_SEARCH_ITEM  search_item[]=
{
    {ID_SIGNATURE,         "IMAGE_SIGNATURE",          GT_FALSE},
    {ID_PRODUCT_CODE,      "IMAGE_PRODUCT_CODE",       GT_FALSE},
    {ID_COMPRESS_METHOD,   "IMAGE_COMPRESS_METHOD",    GT_FALSE},
    {ID_VERSION,           "IMAGE_VERSION",            GT_FALSE},
    {ID_DESCRIPTION,       "IMAGE_DESCRIPTION",        GT_FALSE},

	/* image signature header offset in image */
	{ID_SIGNATURE_OFFSET,  "IMAGE_SIG_HEADER_OFFSET",  GT_FALSE},

    /* search item end */
    {ID_MAX,               NULL,                       GT_FALSE}
};

static       GT_UI8            copyBuffer[512];
static       GT_IMAGE_HEADER   image_header;
static       GT_UI32           signature_offset = 0;


static GT_UI32 _hextoi(IN const GT_UI8 *str)
{
	GT_UI32         i, len;
	GT_UI8          ch;
	GT_UI32         val = 0;

	len = strlen(str);
	for (i=0; i<len; i++)
	{
		ch = str[i];

		if (isxdigit(ch))
		{
		    val <<= 4;
		}

		if ((ch >= '0') && (ch <= '9'))
		{
			val |= ch - '0';
		}
		else if ((ch >= 'a') && (ch <= 'f'))
		{
			val |= ch - 'a' + 0xA;
		}
		else if ((ch >= 'A') && (ch <= 'F'))
		{
			val |= ch - 'A' + 0xA;
		}
		else
		{
			break;
		}
	}

	return val;
}

static GT_UI32 _ATOI(IN const GT_UI8 *p)
{
	if (p == NULL)
	{
		return 0;
	}

	if ((p[0] == '0') && ((p[1] == 'x') || (p[1] == 'X')))
	{
		return (GT_UI32)_hextoi(p+2);
	}
	else
    {
	    return (GT_UI32)atoi(p);
	}
}


static void _set_sig_field
(
    IN GT_ID             id,
    IN const GT_UI8     *str,
    OUT GT_IMAGE_HEADER *header
)
{
    GT_UI32     loop;

	switch (id)
	{
		case ID_SIGNATURE:
			header->signature = _ATOI(str);
			break;

		case ID_PRODUCT_CODE:
			header->product_code = _ATOI(str);
			break;

		case ID_COMPRESS_METHOD:
		    header->compress_method = _ATOI(str);
		    break;

		case ID_VERSION:
		    str++;  /* skip the '"' character */
		    for (loop = 0; *str != '"'; loop++, str++)
		    {
		        header->version[loop] = *str;
		    }
			break;

		case ID_DESCRIPTION:
		    str++;  /* skip the '"' character */
		    for (loop = 0; *str != '"'; loop++, str++)
		    {
		        header->description[loop] = *str;
		    }
		    break;

		case ID_SIGNATURE_OFFSET:
			signature_offset = _ATOI(str);
			break;

		default:
		    break;
	}
}

/*
 * return GT_TRUE  : get signature success;
 * return GT_FALSE : get signature fail;
 */
static GT_BOOL _get_sig_info
(
    IN  const GT_I8             *sig_file,
    OUT       GT_IMAGE_HEADER   *header
)
{
	FILE    *p_file;
	GT_UI8   tmp_buf[128 + 1];
	GT_UI8  *p_buf;
	GT_UI32  real_load_byte, load_block_base = 0;
	GT_UI32  loop;
	GT_BOOL  ret = GT_TRUE;

	memset(header,  0, sizeof(*header));

	p_file = fopen(sig_file, "rb");
	if (p_file == NULL)
	{
	    printf("Open Image Signature file %s fail!\n\r", sig_file);
		return GT_FALSE;
	}

	while (1)
	{
        /* re-set the current read file pointer in signature file */
        if (fseek(p_file, load_block_base, SEEK_SET) == -1)
        {
            printf("fseek file_1 %s fail! base=0x%08X, offset=0x%08X \n\r",
                   sig_file, load_block_base, p_buf - tmp_buf);

            fclose(p_file);
	        return GT_FALSE;
        }

	    memset(tmp_buf, 0x0, sizeof(tmp_buf));
	    real_load_byte = fread(tmp_buf, 1, sizeof(tmp_buf)-1, p_file);
	    if (real_load_byte == 0)
	    {
            break;
	    }

        /* first keyword search */
        p_buf = strstr(tmp_buf, SEARCH_KEYWORD_1ST);
	    if (p_buf == NULL)
	    {
	        load_block_base += real_load_byte;
	        continue;
	    }

        /* skip the 1st search keyword */
        p_buf += strlen(SEARCH_KEYWORD_1ST);

        load_block_base += p_buf - tmp_buf;

        /* re-set the current read file pointer in signature file */
        if (fseek(p_file, load_block_base, SEEK_SET) == -1)
        {
            printf("fseek file_2 %s fail! base=0x%08X, offset=0x%08X \n\r",
                   sig_file, load_block_base, p_buf - tmp_buf);

            fclose(p_file);
	        return GT_FALSE;
        }

        /* re-get a new block into buffer,
         * start from the address that just after the 1st search keyword.
         */
        memset(tmp_buf, 0x0, sizeof(tmp_buf));
	    real_load_byte = fread(tmp_buf, 1, sizeof(tmp_buf)-1, p_file);
	    if (real_load_byte == 0)
        {
            break;
        }

        if (0 != strncmp(tmp_buf,
                         SEARCH_KEYWORD_2ND,
                         strlen(SEARCH_KEYWORD_2ND)))
        {
            continue;
        }

        /* skip spaces */
        p_buf = tmp_buf + strlen(SEARCH_KEYWORD_2ND);
        while ((*p_buf == ' ') || (*p_buf == '\t'))
        {
            p_buf++;
        }

        for (loop = 0; search_item[loop].keyword != NULL; loop++)
        {
            if (0 == strncmp(p_buf,
                             search_item[loop].keyword,
                             strlen(search_item[loop].keyword)))
            {
                /* skip the keyword, and all spaces */
                p_buf += strlen(search_item[loop].keyword);
                while ((*p_buf == ' ') || (*p_buf == '\t'))
                {
                    p_buf++;
                }

                _set_sig_field(search_item[loop].id, p_buf, header);
                search_item[loop].valid = GT_TRUE;

                /* move to the end of this line */
                while ((*p_buf != '\n') && (*p_buf != '\r'))
                {
                    p_buf++;
                }

                load_block_base += p_buf - tmp_buf;
	            break;
	        }
        }
	}

	fclose(p_file);

	for (loop = 0; search_item[loop].keyword != NULL; loop++)
	{
	    if (search_item[loop].valid != GT_TRUE)
	    {
	        ret = GT_FALSE;
	        printf(" Signature Config File Error: Lost field %s define!\n\r",
	               search_item[loop].keyword);
	    }
	}

	return ret;
}


/* return : the check sum */
static GT_UI16 _calc_checksum
(
    IN       GT_UI16   sum,
    IN const GT_UI16  *p16,
    IN       GT_UI32   len)
{
	int           i, cnt;

	cnt = len / sizeof(GT_UI16);

	for (i = 0; i < cnt; i++)
	{
		sum += p16[i];
	}

	return sum;
}


int main(int _argc, char *_argv[])
{
	FILE    *pf_in, *pf_out;
    GT_I8   *image_cfg_name, *image_input_name, *image_output_name;
	GT_UI16  sum;
	GT_UI32  i, n, len, offset;

    if (_argc < 4)
	{
		printf("\n\rImage Signature Tool: (Version 1.0)    "__DATE__
		       "\n\r"
               "\n\r Copyright (C) 2007 Free Software Foundation, Inc."
               "\n\r   This tool is free software, covered by the GNU General Public License,"
               "\n\r   and you are welcome to change it and/or distribute copies of it under"
               "\n\r   certain conditions."
		       "\n\r "
		       "\n\r Usage:"
               "\n\r   ImageSig <image_sig filename> <input image filename> <output image filename>"
               "\n\r"
		       "\n\r Author:"
		       "\n\r   Panda Xiong (yaxi1984@gmail.com)"
               "\n\r");
		return 0;
	}

    image_cfg_name    = _argv[1];
    image_input_name  = _argv[2];
    image_output_name = _argv[3];

    /* get image signature information from image config file. */
	if (_get_sig_info(image_cfg_name, &image_header) != GT_TRUE)
	{
		return 0;
    }

	/* open input image file */
	pf_in = fopen(image_input_name, "rb");
	if (pf_in == NULL)
	{
		printf("Open input image file %s fail!\n\r", image_input_name);
		return 0;
	}

	/* create signatured image file */
	pf_out = fopen(image_output_name, "wb+");
	if (pf_out == NULL)
	{
		printf("Create signatured image file %s fail!\n\r", image_output_name);
		fclose(pf_in);
		return 0;
	}

    /* In none compressed mode:
     *   we directly write the image header into image, use overwrite mode.
     * In compressed mode:
     *   before the 'signature_offset' area, we write nothing;
     *   we only write the compressed data after the 'image_header',
     *   alligned to 4Bytes.
     */
    if (image_header.compress_method != 0)
    {
        /* move file pointer to after the 'image_header',
         * and alligned to 4 Bytes.
         */
        offset = signature_offset + sizeof(image_header);
        offset = (offset + 3) & (~3);  /* allign to 4 Bytes */
        fseek(pf_out, offset, SEEK_SET);
    }

	/* copy image file */
	n   = sizeof(copyBuffer);
    sum = 0;
	while (n == sizeof(copyBuffer))
	{
		n = fread(copyBuffer, 1, n, pf_in);

		/* align to 4 byte */
		if (n & 3)
		{
			len = (n + 3) & 0xFFFFFFFC;

			/* append zero */
			memset(copyBuffer + n, 0, 4 - n%4);
		}
		else
		{
			len = n;
		}

		sum = _calc_checksum(sum, (GT_UI16 *)copyBuffer, len);

		fwrite(copyBuffer, 1, len, pf_out);
	}

	fseek(pf_out, 0, SEEK_END);
	image_header.size = ftell(pf_out);   /* set image size */
	fseek(pf_out, 0, SEEK_SET);

	/* compute the image signature header checksum */
    image_header.checksum = 0;
	sum = _calc_checksum(sum, (GT_UI16 *)&image_header, sizeof(image_header));

	/* set checksum */
	image_header.checksum = (GT_UI16)(0x10000 - sum);

	/* write image signature header */
	fseek(pf_out, signature_offset, SEEK_SET);
	fwrite(&image_header, 1, sizeof(image_header), pf_out);

	/* close files */
	fclose(pf_in);
	fclose(pf_out);

    return 0;
}

