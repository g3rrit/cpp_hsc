# hsc (header-source-compiler)

usage:
----------------------------
./hsc hc/cc dest_dir src_dir

----------------------------

###### main.cc
```
#hdr
int main();
#src
int main()
{
  return 0;
}
```

###### main.hpp
```
int main();
```
###### main.cpp
```
int main()
{
  return 0;
}
```
