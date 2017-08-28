#pragma once

namespace s2 {

template <typename Encoding>
struct convert<Encoding, char32_t> {
  static size_t length(char32_t val) {
    null_iterator<typename Encoding::storage_type> it;
    Encoding::encode(it, val);
    return it.count;
  }
  template <typename It>
  static void encode(It &it, char32_t val) {
    Encoding::encode(it, val);
  }
};
template <typename Encoding>
struct convert<Encoding, char16_t> {
  static size_t length(char16_t val) {
    null_iterator<typename Encoding::storage_type> it;
    Encoding::encode(it, val);
    return it.count;
  }
  template <typename It>
  static void encode(It &it, char16_t val) {
    Encoding::encode(it, val);
  }
};
template <typename Encoding>
struct convert<Encoding, char> {
  static size_t length(char val) {
    null_iterator<typename Encoding::storage_type> it;
    Encoding::encode(it, val);
    return it.count;
  }
  template <typename It>
  static void encode(It &it, char val) {
    Encoding::encode(it, val);
  }
};
template <typename Encoding, typename Encoding2>
struct convert<Encoding, s2::basic_string_view<Encoding2>> {
  static size_t length(const s2::basic_string_view<Encoding2>& val) {
    null_iterator<typename Encoding::storage_type> it;
    for (auto&& c : val) {
      Encoding::encode(it, c);
    }
    return it.count;
  }
  template <typename It>
  static void encode(It &it, const s2::basic_string_view<Encoding2>& val) {
    for (auto&& c : val) {
      Encoding::encode(it, c);
    }
  }
};
template <typename Encoding>
struct convert<Encoding, uint64_t> {
  static constexpr char charlist[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static size_t length(uint64_t val) {
    size_t count = 0;
    if (val >= 10) {
      count = length(val / 10);
    }
    null_iterator<typename Encoding::storage_type> it;
    static_assert(10 < sizeof(charlist));
    Encoding::encode(it, charlist[val % 10]);
    count += it.count;
    return count;
  }
  template <typename It>
  static void encode(It &it, uint64_t val) {
    if (val >= 10) {
      encode(it, val / 10);
    }
    Encoding::encode(it, '0' + (val % 10));
  }
};
template <typename Encoding>
struct convert<Encoding, int64_t> {
  static size_t length(int64_t val) {
    size_t count = 0;
    uint64_t abs_x = (uint64_t)val;
    if (val < 0) {
      null_iterator<typename Encoding::storage_type> it;
      Encoding::encode(it, '-');
      count += it.count;
      abs_x = 0ull - abs_x;
    }
    count += convert<Encoding, uint64_t>::length(abs_x);
    return count;
  }
  template <typename It>
  static void encode(It &it, int64_t val) {
    uint64_t abs_x = (uint64_t)val;
    if (val < 0) {
      Encoding::encode(it, '-');
      abs_x = 0ull - abs_x;
    }
    convert<Encoding, uint64_t>::encode(it, abs_x);
  }
};

template <typename Encoding> struct convert<Encoding, unsigned int> : convert<Encoding, uint64_t> {};
template <typename Encoding> struct convert<Encoding, unsigned short> : convert<Encoding, uint64_t> {};
template <typename Encoding> struct convert<Encoding, unsigned char> : convert<Encoding, uint64_t> {};
template <typename Encoding> struct convert<Encoding, signed int> : convert<Encoding, int64_t> {};
template <typename Encoding> struct convert<Encoding, signed short> : convert<Encoding, int64_t> {};
template <typename Encoding> struct convert<Encoding, signed char> : convert<Encoding, int64_t> {};

}


