# Cerpent

A basic C interpreter, because Pythons are gross.

Example:

```c
Cerpent v1.0
> printf("Hello World!\n");
Hello World!
> int add(int a, int b) {
    return a + b;
  }
> int res = add(5, 25);
> printf("%d\n", res);
30
```

Run `*.cerp` files:

```c
$ cat count_to_ten.cerp
int i = 1;
for (; i <= 10; ++i) {
  printf("%d, ", i);
}
printf("\n");
$ cerp count_to_ten.cerp
1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
```

## Build Requirements

- `cmake >= 3.1.0`
- `clang >= 6.0`

```bash
sudo apt install clang-6.0 libclang-6.0-dev
export CC=clang-6.0
export CXX=clang++-6.0
```
