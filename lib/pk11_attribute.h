/* vi: set sw=4 ts=4:
 *
 * Copyright (C) 2001 - 2007 Christian Hohnstaedt.
 *
 * All rights reserved.
 */

#ifndef PKI_PKCS11_ATTRIBUTE_H
#define PKI_PKCS11_ATTRIBUTE_H

#include <qstring.h>
#include <stdlib.h>
#include <openssl/bn.h>
#include "opensc-pkcs11.h"

#define UTF8QSTRING(x,s) QString::fromUtf8((const char*)(x), s).trimmed();

class pk11_attlist;

class pk11_attribute
{
	friend class pk11_attlist;
protected:
	CK_ATTRIBUTE attr;

public:
	pk11_attribute(unsigned long type)
	{
		memset(&attr, 0, sizeof(attr));
		attr.type = type;
	}
	virtual ~pk11_attribute() { }
	const CK_ATTRIBUTE *getAttribute() const
	{
		return &attr;
	}
	virtual void store(CK_SESSION_HANDLE sess, CK_OBJECT_HANDLE obj);
	virtual void load(CK_SESSION_HANDLE sess, CK_OBJECT_HANDLE obj);
};

class pk11_attr_bool: public pk11_attribute
{
protected:
	unsigned char value;

public:
	pk11_attr_bool(unsigned long type, bool v = false)
			:pk11_attribute(type)
	{
		attr.pValue = &value;
		attr.ulValueLen = sizeof(value);
		setValue(v);
	}
	bool getValue() const
	{
		return value ? true : false;
	}
	void setValue(unsigned long v)
	{
		value = v ? 1 : 0;
	}
};

class pk11_attr_ulong: public pk11_attribute
{
protected:
	unsigned long value;

public:
	pk11_attr_ulong(unsigned long type, unsigned long v = 0)
			:pk11_attribute(type)
	{
		attr.pValue = &value;
		attr.ulValueLen = sizeof(value);
		setValue(v);
	}
	unsigned long getValue() const
	{
		return value;
	}
	void setValue(unsigned long v)
	{
		value = v;
	}
};

class pk11_attr_data: public pk11_attribute
{

public:
	pk11_attr_data(unsigned long type, const unsigned char *v = NULL,
			unsigned long len = 0) :pk11_attribute(type)
	{
		setValue(v, len);
	}
	unsigned long getValue(const unsigned char **ptr)
	{
		*ptr = (unsigned char*)attr.pValue;
		return attr.ulValueLen;
	}
	~pk11_attr_data()
	{
		if (attr.pValue)
			free(attr.pValue);
	}
	QString getText() const
	{
		return UTF8QSTRING(attr.pValue, attr.ulValueLen);
	}
	BIGNUM *getBignum() const
	{
		return BN_bin2bn((unsigned char*)attr.pValue,
				attr.ulValueLen, NULL);
	}
	void load(CK_SESSION_HANDLE sess, CK_OBJECT_HANDLE obj);
	void setValue(const unsigned char *ptr, unsigned long len);
};

class pk11_attlist {

	private:
		CK_ATTRIBUTE *attributes;
		unsigned long attlen;
		unsigned long alloc_len;
		void init()
		{
			attlen = 0;
			alloc_len = 0;
			attributes = NULL;
		}

	public:
		pk11_attlist()
		{
			init();
		}
		pk11_attlist(const pk11_attlist &a);
		pk11_attlist(const pk11_attribute &a)
		{
			init();
			addAttribute(a);
		}
		~pk11_attlist();
		unsigned long get(CK_ATTRIBUTE **attp)
		{
			*attp = attributes;
			return attlen;
		}
		void addAttribute(const pk11_attribute &a);
		pk11_attlist &operator << (const pk11_attribute &a)
		{
			addAttribute(a);
			return *this;
		}
		void reset();
};
#endif