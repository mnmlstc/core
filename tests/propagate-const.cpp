#include <core/propagate_const.hpp>
#include <memory>

#include "catch.hpp"

TEST_CASE("default-constructor", "[constructors]") {
  core::propagate_const<std::unique_ptr<int>> value { };

  CHECK_FALSE(value);
}
