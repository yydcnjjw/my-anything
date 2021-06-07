#include <org/parser/config.hpp>
#include <org/parser/grammar/common_def.hpp>
#include <org/parser/grammar/greater_block_def.hpp>
#include <org/parser/grammar/paragraph_def.hpp>
#include <org/parser/grammar/headline_def.hpp>
#include <org/parser/grammar/section_def.hpp>
#include <org/parser/grammar/content_def.hpp>
#include <org/parser/grammar/document_def.hpp>
#include <org/parser/parser.hpp>

namespace my {
namespace org {

namespace grammar {

BOOST_SPIRIT_INSTANTIATE(document_t, stream_iterator_type,
                         context_t<stream_iterator_type>);
BOOST_SPIRIT_INSTANTIATE(document_t, iterator_type, context_t<iterator_type>);

BOOST_SPIRIT_INSTANTIATE(headline_t, stream_iterator_type,
                         context_t<stream_iterator_type>);
BOOST_SPIRIT_INSTANTIATE(headline_t, iterator_type, context_t<iterator_type>);

BOOST_SPIRIT_INSTANTIATE(section_t, stream_iterator_type,
                         context_t<stream_iterator_type>);
BOOST_SPIRIT_INSTANTIATE(section_t, iterator_type,
                         context_t<iterator_type>);
  
} // namespace grammar

} // namespace org

} // namespace my
