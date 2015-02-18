#ifndef ROOT_PATH_KEY_H_
#define ROOT_PATH_KEY_H_

struct RootPath {
  RootPath(long _root, short _path) : root(_root), path(_path) {}

  long root;
  short path;
};

struct RootPathFunc {
  size_t operator() (const RootPath& a) const {
    return (a.root << 4) | a.path;
  }

  bool operator() (const RootPath& a, const RootPath& b) const {
    return (a.root == b.root
                && a.path == b.path);
  }
};


#endif  // ROOT_PATH_KEY_H_
