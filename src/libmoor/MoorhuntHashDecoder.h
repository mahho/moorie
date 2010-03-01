/*
 * This file is a part of Moorie
 *
 * Copyright (C) 2007-2010 Moorie Team - http://moorie.pl/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef MOORHUNTHASHDECODER_H__
#define MOORHUNTHASHDECODER_H__

#include <string>

#include "HashDecoder.h"
#include "Tools.h"


class MoorhuntHashDecoder : public HashDecoder {
	public:
		~MoorhuntHashDecoder() {}

	public:
		virtual Hash* decode(const std::string& hashcode);
};

class MoorhuntHashEncoder {
	    std::string myFilename;
	    int myCRC;
		std::string myCleanCRC;
	    int myFileSize;
	    bool myRound;
	    bool myRandName;
	    int mySegmentCount;
	    int myMinSize;
	    int myMaxSize;
	    std::string myDownloadPassword;
	    int myMirrors;
	    // tutaj lista mirrorow
	    std::string myAvartLink;
	    std::string myEditPassword;
	    std::string myFor;
	    std::string myFullDescriptionLink;
	    std::string myFullTitle;
	    std::string myUploader;
	    std::string myComment;
		std::vector<std::string> mailboxes;


	public:
		MoorhuntHashEncoder();
		~MoorhuntHashEncoder();
		void setFilename(std::string data) { myFilename = data; };
		void setCRC(std::string data) { myCleanCRC = data; };
		void setFileSize(int data) { myFileSize = data; };
		void setRound(bool data) { myRound = data; };
		void setRandName(bool data) { myRandName = data; };
		void setSegmentCount(int data) { mySegmentCount = data; };
		void setSegSize(int data) { myMinSize = data; myMaxSize = data; };
		void setDownloadPassword(std::string data) { myDownloadPassword = data.c_str(); };
		void setMirrors(int data) { myMirrors = data; };
		void setAvartLink(std::string data) { myAvartLink = data; };
		void setEditPassword(std::string data) { myEditPassword = data.c_str(); };

		const bool decode(std::string hashcode);
		const std::string encodeClean();
		const std::string encodeWithNewMirror();
		const std::string addNewMirror(std::string editpassword, std::string hashcode, std::string mailbox, std::string password);

		std::string getMD5(unsigned char *string);
};


#endif // MOORHUNTHASHDECODER_H__

