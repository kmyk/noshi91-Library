#include <cassert>
#include <cstddef>
#include <vector>

template <class Monoid> class dual_segment_tree {
  using size_t = std::size_t;
  using T = typename Monoid::value_type;

public:
  using value_type = T;
  using size_type = size_t;

private:
  static size_t lsb(const size_t x) { return __builtin_ctz(x); }
  static size_t msb(const size_t x) { return 31 - __builtin_clz(x); }
  static void add(T &x, const T y) { x = Monoid::operation(x, y); }

  std::vector<T> tree;

  void push(const size_t index) {
    add(tree[index * 2], tree[index]);
    add(tree[index * 2 + 1], tree[index]);
    tree[index] = Monoid::identity;
  }
  void propagate(const size_t index) {
    if (index == 0)
      return;
    const size_t lsb_ = lsb(index);
    for (size_t h = msb(index); h != lsb_; h -= 1)
      push(index >> h);
  }

public:
  dual_segment_tree() = default;
  explicit dual_segment_tree(const size_t n) : tree(n * 2, Monoid::identity) {}

  size_t size() const noexcept { return tree.size() / 2; }

  T fold(size_t index) const {
    assert(index < size());
    index += size();
    T ret = tree[index];
    while (index != 1) {
      index /= 2;
      add(ret, tree[index]);
    }
    return ret;
  }

  void update(size_t first, size_t last, const T x) {
    assert(first <= last);
    assert(last <= size());
    first += size();
    last += size();
    propagate(first);
    propagate(last);
    while (first != last) {
      if (first % 2 != 0) {
        add(tree[first], x);
        first += 1;
      }
      first /= 2;
      if (last % 2 != 0) {
        last -= 1;
        add(tree[last], x);
      }
      last /= 2;
    }
  }
};