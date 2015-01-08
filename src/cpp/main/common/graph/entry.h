#ifndef ENTRY_H_
#define ENTRY_H_

struct Entry {
  Entry();
  Entry(long _id, double _value);

  static const Entry& getNullItem();

  long id;
  double value;
  bool operator==(const Entry& entry) const;

private:
  static Entry nullItem;
};

#endif // ENTRY_H_
