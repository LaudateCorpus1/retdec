/**
 * @file src/config/vtables.cpp
 * @brief Decompilation configuration manipulation: vtables.
 * @copyright (c) 2017 Avast Software, licensed under the MIT license
 */

#include "retdec/config/vtables.h"
#include "retdec/serdes/address.h"

namespace {

const std::string JSON_name          = "name";
const std::string JSON_address       = "address";
const std::string JSON_targetAddress = "targetAddress";
const std::string JSON_targetName    = "targetName";
const std::string JSON_items         = "items";

} // anonymous namespace

namespace retdec {
namespace config {

//
//=============================================================================
// VtableItem
//=============================================================================
//

VtableItem::VtableItem(const retdec::common::Address& a) :
		_address(a)
{

}

/**
 * Reads JSON object (associative array) holding virtual function
 * table item information.
 * @param val JSON object.
 */
VtableItem VtableItem::fromJsonValue(const Json::Value& val)
{
	checkJsonValueIsObject(val, "VtableItem");

	common::Address a;
	serdes::deserialize(val[JSON_address], a);
	VtableItem ret(a);

	serdes::deserialize(val[JSON_targetAddress], ret._targetAddress);
	ret.setTargetFunctionName( safeGetString(val, JSON_targetName) );

	return ret;
}

/**
 * Returns JSON object (associative array) holding virtual function
 * table item information.
 * @return JSON object.
 */
Json::Value VtableItem::getJsonValue() const
{
	Json::Value val;

	if (getAddress().isDefined()) val[JSON_address] = serdes::serialize(getAddress());
	if (getTargetFunctionAddress().isDefined()) val[JSON_targetAddress] = serdes::serialize(getTargetFunctionAddress());
	if (!getTargetFunctionName().empty()) val[JSON_targetName] = getTargetFunctionName();

	return val;
}

void VtableItem::setTargetFunctionAddress(const retdec::common::Address& a)
{
	_targetAddress = a;
}

void VtableItem::setTargetFunctionName(const std::string& n)
{
	_targetFunctionName = n;
}

retdec::common::Address VtableItem::getId() const
{
	return getAddress();
}

/**
 * @return Virtual table item's address in binary file.
 */
retdec::common::Address VtableItem::getAddress() const
{
	return _address;
}

/**
 * @return Virtual function address for this item.
 */
retdec::common::Address VtableItem::getTargetFunctionAddress() const
{
	return _targetAddress;
}

/**
 * @return Name (unique ID) of function on target address.
 */
std::string VtableItem::getTargetFunctionName() const
{
	return _targetFunctionName;
}

/**
 * Virtual table items are ordered by their addresses in binary file.
 */
bool VtableItem::operator<(const VtableItem& o) const
{
	return getAddress() < o.getAddress();
}

/**
 * Two virtual table items are equal if their addresses in binary file
 * are equal.
 */
bool VtableItem::operator==(const VtableItem& o) const
{
	return getAddress() == o.getAddress();
}

//
//=============================================================================
// Vtable
//=============================================================================
//

Vtable::Vtable(const retdec::common::Address& a)
{
	_address = a;
}

/**
 * Reads JSON object (associative array) holding virtual function
 * table information.
 * @param val JSON object.
 */
Vtable Vtable::fromJsonValue(const Json::Value& val)
{
	checkJsonValueIsObject(val, "Vtable");

	common::Address a;
	serdes::deserialize(val[JSON_address], a);
	Vtable ret(a);

	ret.setName( safeGetString(val, JSON_name) );
	ret.items.readJsonValue(val[JSON_items]);

	return ret;
}

/**
 * Returns JSON object (associative array) holding virtual function
 * table information.
 * @return JSON object.
 */
Json::Value Vtable::getJsonValue() const
{
	Json::Value val;

	if (!getName().empty()) val[JSON_name] = getName();
	if (getAddress().isDefined()) val[JSON_address] = serdes::serialize(getAddress());
	if (!items.empty()) val[JSON_items] = items.getJsonValue();

	return val;
}

void Vtable::setName(const std::string& n)
{
	_name = n;
}

retdec::common::Address Vtable::getId() const
{
	return getAddress();
}

/**
 * @return Virtual table's address in binary file.
 */
retdec::common::Address Vtable::getAddress() const
{
	return _address;
}

std::string Vtable::getName() const
{
	return _name;
}

/**
 * Virtual tables are ordered by their addresses in binary file.
 */
bool Vtable::operator<(const Vtable& o) const
{
	return getAddress() < o.getAddress();
}

/**
 * Two virtual tables are equal if their addresses in binary file
 * are equal.
 */
bool Vtable::operator==(const Vtable& o) const
{
	return getAddress() == o.getAddress();
}

//
//=============================================================================
// VtableContainer
//=============================================================================
//

} // namespace config
} // namespace retdec
