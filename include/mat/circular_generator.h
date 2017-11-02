#ifndef MAT_CIRCULAR_GENERATOR_H
#define MAT_CIRCULAR_GENERATOR_H


#include <type_traits>
#include <stdexcept>
#include <cassert>


namespace mat::impl
{
    template <typename Sequence>
    struct StlContainerHolder
    {
        using holder_t = std::conditional_t<
                                std::is_lvalue_reference_v<Sequence>,
                                std::add_const_t<Sequence>,
                                std::remove_const_t<Sequence>
                         >;
        using iterator_t = typename std::remove_reference_t<Sequence>::const_iterator;

        holder_t data_;        
        iterator_t const begin_;
        iterator_t const end_;
        iterator_t currentElement_;

        template <typename T>
        StlContainerHolder(T && seq)
            : data_{ std::forward<T>(seq) }
            , begin_{ data_.begin() }
            , end_{ data_.end() }
            , currentElement_{ begin_ }
        {
            if (std::distance(begin_, end_) == 0) {
                throw std::range_error("empty sequence is provided");
            }
        }

        auto next()
        {
            assert(std::distance(begin_, end_) != 0);
            if (currentElement_ == end_) {
                currentElement_ = begin_;
            }
            return *(currentElement_++);
        }
    };
} // namespace mat::impl


namespace mat
{
    template
    <
        typename Sequence,
        template <typename> class DataHolder,
        typename = void
    >
    class circular_generator : DataHolder<Sequence>
    {
    public:
        template <typename T>
        circular_generator(T && seq)
            : DataHolder<Sequence>{ std::forward<T>(seq) }
        { }

    public:
        auto operator () ()
        { return DataHolder<Sequence>::next(); }
    };

    template <typename Sequence>
    circular_generator(Sequence &&) -> circular_generator<
                                            Sequence,
                                            mat::impl::StlContainerHolder,
                                            typename std::decay_t<Sequence>::value_type
                                       >;

} // namespace mat


#endif // MAT_CIRCULAR_GENERATOR_H
