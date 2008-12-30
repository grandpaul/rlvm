/*
  rlBabel: abstract base class for codepage definitions

  Copyright (c) 2006 Peter Jolly.

  This library is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation; either version 2.1 of the License, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  As a special exception to the GNU Lesser General Public License (LGPL), you
  may include a publicly distributed version of the library alongside a "work
  that uses the Library" to produce a composite work that includes the library,
  and distribute that work under terms of your choice, without any of the
  additional requirements listed in clause 6 of the LGPL.

  A "publicly distributed version of the library" means either an unmodified
  binary as distributed by Haeleth, or a modified version of the library that is
  distributed under the conditions defined in clause 2 of the LGPL, and a
  "composite work that includes the library" means a RealLive program which
  links to the library, either through the LoadDLL() interface or a #DLL
  directive, and/or includes code from the library's Kepago header.

  Note that this exception does not invalidate any other reasons why any part of
  the work might be covered by the LGPL.
*/

#include "codepage.h"

// Supported codepages
#include "cp932.h"
#include "cp936.h"
#include "cp949.h"
#include "western.h"


// -----------------------------------------------------------------------
// Codepage
// -----------------------------------------------------------------------
Codepage::~Codepage() {
	// empty virtual destructor
}

unsigned short Codepage::JisDecode(unsigned short ch) const {
	return ch;
}

void Codepage::JisEncodeString(const char* src, char* buf, size_t buflen) const {
	strncpy((char*) buf, (const char*) src, buflen);
}

void Codepage::JisDecodeString(const char* src, char* buf, size_t buflen) const {
	int srclen = strlen(src), i = 0, j = 0;
	while (i < srclen && j < buflen) {
		unsigned int c1 = (unsigned char) src[i++];
		if ((c1 >= 0x81 && c1 < 0xa0) || (c1 >= 0xe0 && c1 < 0xf0))
			c1 = (c1 << 8) | (unsigned char) src[i++];
		unsigned int c2 = JisDecode(c1);
		if (c2 <= 0xff)
			buf[j++] = c2;
		else {
			buf[j++] = (c2 >> 8) & 0xff;
			buf[j++] = c2 & 0xff;
		}
	}
	buf[j] = 0;
}

unsigned short Codepage::Convert(unsigned short ch) const {
	return ch;
}

bool Codepage::DbcsDelim(char* str) const {
	return false;
}

bool Codepage::IsItalic(unsigned short ch) const {
	return false;
}

boost::scoped_ptr<Codepage> Cp::instance_;
int Cp::codepage = -1;
int Cp::scenario = -1;

Codepage& Cp::instance(int desired) {
  // Desired codepage known.
  if (desired != codepage) {
    codepage = desired;
    switch (desired) {
      case 1:
        instance_.reset(new Cp936());
        break;
      case 2:
        instance_.reset(new Cp1252());
        break;
      case 3:
        instance_.reset(new Cp949());
        break;
      default:
        instance_.reset(new Cp932());
    }
  }
  return *instance_;
}
