A Device Family Pack created in a coding session lasting from like 12:00 AM to 2:30 AM

```sh
cmake -B build -DSOURCES="examples/blink.c"
cmake --build build
cmake --build build --target flash
```

TODO:

[ ] fix USB serial