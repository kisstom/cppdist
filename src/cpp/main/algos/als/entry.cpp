#include "entry.h"

Entry::Entry() : id(0), value(0.0) {}
Entry::Entry(long _id, double _value) : id(_id), value(_value) {}

bool Entry::operator==(const Entry& entry) const {
  if (entry.id != id) return false;
  if (entry.value != value) return false;
  return true;
}

const Entry& Entry::getNullItem() {
  return nullItem;
}

Entry Entry::nullItem(-1, -1.0);

