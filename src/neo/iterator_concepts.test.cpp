#include <neo/iterator_concepts.hpp>

#include <neo/concepts.hpp>

#include "./test_concept.hpp"

#include <forward_list>
#include <memory>

using namespace neo;

NEO_TEST_CONCEPT(same_as<iter_value_t<int*>, int>);
NEO_TEST_CONCEPT(same_as<iter_value_t<const int*>, int>);
NEO_TEST_CONCEPT(same_as<iter_value_t<std::string>, char>);
NEO_TEST_CONCEPT(same_as<iter_value_t<const std::string>, char>);
NEO_TEST_CONCEPT(same_as<iter_value_t<std::shared_ptr<int>>, int>);
NEO_TEST_CONCEPT(same_as<iter_value_t<std::shared_ptr<std::string>>, std::string>);

NEO_TEST_CONCEPT(same_as<iter_reference_t<int*>, int&>);
NEO_TEST_CONCEPT(same_as<iter_reference_t<const int*>, const int&>);

NEO_TEST_CONCEPT(same_as<iter_difference_t<int*>, std::ptrdiff_t>);

NEO_TEST_CONCEPT(indirectly_readable<int*>);
NEO_TEST_CONCEPT(indirectly_readable<std::string::iterator>);
NEO_TEST_CONCEPT(indirectly_readable<std::shared_ptr<int>>);

NEO_TEST_CONCEPT(indirectly_writable<std::shared_ptr<int>, int>);
NEO_TEST_CONCEPT(indirectly_writable<int*, int>);
NEO_TEST_CONCEPT(!indirectly_writable<const int*, int>);  // Cannot write-to-const
NEO_TEST_CONCEPT(indirectly_writable<std::string::iterator, char>);
NEO_TEST_CONCEPT(!indirectly_writable<std::string::const_iterator, char>);

NEO_TEST_CONCEPT(weakly_incrementable<int*>);
NEO_TEST_CONCEPT(weakly_incrementable<std::string::const_iterator>);
NEO_TEST_CONCEPT(incrementable<int*>);
NEO_TEST_CONCEPT(incrementable<std::string::const_iterator>);

NEO_TEST_CONCEPT(input_or_output_iterator<int*>);
NEO_TEST_CONCEPT(input_or_output_iterator<std::string::const_iterator>);

NEO_TEST_CONCEPT(sentinel_for<int*, int*>);
NEO_TEST_CONCEPT(sentinel_for<std::string::const_iterator, std::string::const_iterator>);
NEO_TEST_CONCEPT(sentinel_for<std::string::iterator, std::string::const_iterator>);
NEO_TEST_CONCEPT(sentinel_for<std::string::iterator, std::string::iterator>);
NEO_TEST_CONCEPT(sentinel_for<std::string::const_iterator, std::string::iterator>);
NEO_TEST_CONCEPT(!sentinel_for<std::string::const_iterator, int*>);
NEO_TEST_CONCEPT(!sentinel_for<int*, std::string::const_iterator>);

NEO_TEST_CONCEPT(input_iterator<int*>);
NEO_TEST_CONCEPT(input_iterator<std::string::iterator>);
NEO_TEST_CONCEPT(output_iterator<std::string::iterator, char>);
NEO_TEST_CONCEPT(!output_iterator<std::string::const_iterator, char>);

NEO_TEST_CONCEPT(forward_iterator<int*>);
NEO_TEST_CONCEPT(forward_iterator<std::forward_list<int>::iterator>);

NEO_TEST_CONCEPT(bidirectional_iterator<std::vector<int>::iterator>);
NEO_TEST_CONCEPT(!bidirectional_iterator<std::forward_list<int>::iterator>);

NEO_TEST_CONCEPT(random_access_iterator<std::vector<int>::iterator>);

// No stdlib types have contiguous iterators yet, but we can make-do with pointers
NEO_TEST_CONCEPT(contiguous_iterator<int*>);
NEO_TEST_CONCEPT(!contiguous_iterator<std::forward_list<int>::iterator>);
