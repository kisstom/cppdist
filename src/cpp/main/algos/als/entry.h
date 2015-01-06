struct Entry {
  Entry(long _id, double _value) : id(_id), value(_value) {}

  long id;
  double value;
  bool operator==(const Entry& entry) const;
};

bool Entry::operator==(const Entry& entry) const {
  if (entry.id != id) return false;
  if (entry.value != value) return false;
  return true;
}
