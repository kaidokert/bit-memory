/**
 * \file named_allocator.hpp
 *
 * \brief This header contains a composite operator that allows naming
 *        of the underlying allocator
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
 */
#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_NAMED_ALLOCATOR_HPP
#define BIT_MEMORY_ALLOCATORS_DETAIL_NAMED_ALLOCATOR_HPP

namespace bit {
  namespace memory {
    namespace detail {

      /////////////////////////////////////////////////////////////////////////
      /// \brief A wrapper around allocators to provide a name to their
      ///        allocator info.
      ///
      /// The underlying allocator must not be marked 'final', otherwise
      /// this will not compile.
      ///
      /// \tparam Allocator the underlying Allocator
      /////////////////////////////////////////////////////////////////////////
      template<typename Allocator>
      class named_allocator : public Allocator
      {
        //---------------------------------------------------------------------
        // Constructors
        //---------------------------------------------------------------------
      public:

        /// \brief constructs a named allocator
        ///
        /// \param str the string name of the allocator
        /// \param args arguments to forward to the underlying allocator
        template<typename...Args, typename = std::enable_if_t<std::is_constructible<Allocator,Args...>::value>>
        named_allocator( const char* str, Args&&...args );

        //---------------------------------------------------------------------
        // Observers
        //---------------------------------------------------------------------
      public:

        /// \brief Gets the info about this allocator
        ///
        /// \return the info for this allocator
        allocator_info info() const noexcept;

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        const char* m_name;
      };

    } // namespace detail
  } // namespace memory
} // namespace bit

#include "named_allocator.inl"

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_NAMED_ALLOCATOR_HPP */