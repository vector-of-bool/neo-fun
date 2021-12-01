#include "./repr.hpp"

#include <ostream>

void neo::repr_detail::item_repr_base::write_ostream(std::ostream& out) const noexcept {
    auto str = this->string();
    out.write(str.data(), static_cast<std::streamsize>(str.size()));
}
