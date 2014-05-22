#include <core/variant.hpp>

#include <unordered_map>
#include <string>
#include <vector>

#include <cstdint>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("variant") = {
    task("default-constructor") = [] {
      core::variant<std::string, int> variant { };
      assert::equal(typeid(std::string), variant.type());
    },

    task("value-constructor") = [] {
      using variant_type = core::variant<std::string, std::uint64_t>;

      variant_type string { "value-constructor" };
      variant_type integer { 64 };

      assert::equal(string.type(), typeid(std::string));
      assert::equal(integer.type(), typeid(std::uint64_t));

      assert::equal(std::get<0>(string), std::string { "value-constructor" });
      assert::equal(std::get<1>(integer), 64u);
    },

    task("move-constructor") = [] {
      using variant_type = core::variant<std::string, double>;
      variant_type variant { std::string { "move" } };
      variant_type move { std::move(variant) };

      assert::equal(move.type(), typeid(std::string));
      assert::equal(std::get<0>(move), std::string { "move" });
      assert::is_true(std::get<0>(variant).empty());
    },

    task("copy-constructor") = [] {
      using variant_type = core::variant<std::string, double>;
      variant_type variant { 0.8 };
      variant_type copy { variant };

      assert::equal(copy.type(), typeid(double));
      assert::equal(variant, copy);
    },

    task("value-assignment-operator") = [] {
      using variant_type = core::variant<std::string, double>;
      std::string string { "value" };
      double real { 0.6 };

      variant_type first { };
      variant_type second { };

      first = string;

      assert::equal(first.type(), typeid(std::string));
      assert::equal(std::get<0>(first), string);

      second = real;

      assert::equal(second.type(), typeid(double));
      assert::equal(std::get<1>(second), real);
    },

    task("move-assignment-operator") = [] {
      using variant_type = core::variant<std::string, double>;
      variant_type variant { "move" };
      variant_type move { 9.8 };

      move = std::move(variant);

      assert::equal(move.type(), typeid(std::string));
      assert::equal(std::get<0>(move), std::string { "move" });
      assert::is_true(std::get<0>(variant).empty());
    },

    task("copy-assignment-operator") = [] {
      using variant_type = core::variant<std::string, double>;
      variant_type variant { 9.8 };
      variant_type copy { };

      copy = variant;

      assert::equal(copy.type(), typeid(double));
      assert::equal(std::get<1>(copy), 9.8);
      assert::equal(copy, variant);
    },

    task("visit") = [] {
      using variant_type = core::variant<
        std::uint64_t,
        std::string,
        std::vector<int>
      >;

      variant_type vector { std::vector<int> { 1, 2, 3 } };

      struct visitor final {
        std::string operator ()(std::string const& str) const { return str; }
        std::string operator ()(std::uint64_t const& val) const {
          return std::to_string(val);
        }
        std::string operator ()(std::vector<int> const& vec) const {
          auto value = vec.at(0) + vec.at(1) + vec.at(2);
          return std::to_string(value);
        }
      };

      auto str = vector.visit(visitor { });
      assert::equal(str, std::string { "6" });
    },

    task("match") = [] {
      using variant_type = core::variant<
        std::uint64_t,
        std::string,
        std::vector<std::string>
      >;

      variant_type vector { std::vector<std::string> { "1", "2", "3" } };
      variant_type string { std::string { "match" } };
      variant_type integer { };

      assert::is_true(
        vector.match(
          [](std::vector<std::string> const&) { return true; },
          [](std::uint64_t const&) { return false; },
          [](std::string const&) { return false; }
        )
      );

      assert::is_true(
        integer.match(
          [](std::vector<std::string> const&) { return false; },
          [](std::uint64_t const&) { return true; },
          [](std::string const&) { return false; }
        )
      );

      assert::is_true(
        string.match(
          [](std::vector<std::string> const&) { return false; },
          [](std::uint64_t const&) { return false; },
          [](std::string const&) { return true; }
        )
      );
    },

    task("which") = [] {
      using variant_type = core::variant<
        std::uint64_t,
        std::string,
        std::vector<std::string>
      >;

      variant_type vector { std::vector<std::string> { "1", "2", "3" } };
      variant_type string { std::string { "which" } };
      variant_type integer { };

      assert::equal(integer.which(), 0u);
      assert::equal(string.which(), 1u);
      assert::equal(vector.which(), 2u);
    },

    task("empty") = [] {
      core::variant<std::uint64_t, float> variant { };
      assert::is_false(variant.empty());
    },

    task("type") = [] {
      using variant_type = core::variant<
        std::uint64_t,
        std::string,
        std::vector<std::string>
      >;

      variant_type vector { std::vector<std::string> { "1", "2", "3" } };
      variant_type string { std::string { "type" } };
      variant_type integer { };

      assert::equal(typeid(std::vector<std::string>), vector.type());
      assert::equal(typeid(std::uint64_t), integer.type());
      assert::equal(typeid(std::string), string.type());
    },

    task("get") = [] {
      using variant_type = core::variant<
        std::uint64_t,
        std::string,
        std::vector<std::string>
      >;

      variant_type vector { std::vector<std::string> { "1", "2", "3" } };
      variant_type string { std::string { "get" } };
      variant_type integer { static_cast<std::uint64_t>(64) };

      assert::throws<core::bad_variant_get>([&vector] {
        std::ignore = std::get<0>(vector);
      });

      assert::equal(std::get<0>(integer), 64u);
      assert::equal(std::get<1>(string), std::string { "get" });
      assert::equal(std::get<2>(vector)[0], std::string { "1" });
      assert::equal(std::get<2>(vector)[1], std::string { "2" });
      assert::equal(std::get<2>(vector)[2], std::string { "3" });
    },

    task("equality-comparable") = [] {
      using variant_type = core::variant<
        std::string,
        double
      >;

      variant_type lhs { std::string { "equality-comparable" } };
      variant_type rhs { std::string { "equality-comparable" } };
      variant_type frhs { 0.7 };

      assert::equal(lhs, rhs);
      assert::is_false(lhs == frhs);
    },

    task("less-than-comparable") = [] {
      using variant_type = core::variant<
        std::string,
        double
      >;

      variant_type lhs { 0.2 };
      variant_type rhs { 0.4 };
      variant_type frhs { std::string { "less-than-comaprable" } };

      assert::less(lhs, rhs);
      assert::is_false(lhs < frhs);
    },

    task("swap") = [] {
      using std::swap;
      using variant_type = core::variant<std::string, double>;
      variant_type lhs_str { std::string { "lhs" } };
      variant_type rhs_str { std::string { "rhs" } };
      variant_type lhs { 0.8 };

      swap(lhs_str, rhs_str);
      assert::equal(std::get<0>(lhs_str), std::string { "rhs" });
      assert::equal(std::get<0>(rhs_str), std::string { "lhs" });

      swap(lhs, rhs_str);
      assert::equal(lhs.type(), typeid(std::string));
      assert::equal(rhs_str.type(), typeid(double));
      assert::equal(std::get<0>(lhs), std::string { "lhs" });
      assert::equal(std::get<1>(rhs_str), 0.8);
    },

    task("hash") = [] {
      using variant_type = core::variant<std::string, double>;
      using unordered_map = std::unordered_map<variant_type, int>;

      unordered_map map;
      map[variant_type { "hash" }] = 9;
      map[variant_type { 9.8 }] = 12;

      assert::equal(map[variant_type { "hash" }], 9);
      assert::equal(map[variant_type { 9.8 }], 12);
    }
  };

  monitor::run();
}
