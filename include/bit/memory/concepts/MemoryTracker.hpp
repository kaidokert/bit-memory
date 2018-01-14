/**
 * \file MemoryTracker.hpp
 *
 * \brief This header defines the concept for a MemoryTracker
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_MEMORY_CONCEPTS_MEMORYTRACKER_HPP
#define BIT_MEMORY_CONCEPTS_MEMORYTRACKER_HPP

#include "detail/void_t.hpp" // detail::void_t

#include "../utilities/allocator_info.hpp" // allocator_info

#include <cstddef>     // std::size_t
#include <type_traits> // std::declval

namespace bit {
  namespace memory {

    ///////////////////////////////////////////////////////////////////////////
    /// \concept{MemoryTracker}
    ///
    /// \brief This concept defines a manner of tracking allocations and
    ///        deallocations to determine leaks and double-deletions
    ///
    /// For type \c T to be \c MemoryTracker, it must satisfy the following:
    ///
    /// **Provided**
    ///
    /// - \c T - a \c MemoryTracker type
    /// - \c t - an instance of type \c T
    /// - \c s - the size of an allocation
    /// - \c a - the alignment of the allocation
    /// - \c v - a void pointer
    /// - \c i - an allocator_info object
    ///
    /// The following expressions must be well-formed and with the following
    /// side-effects:
    ///
    /// \code
    /// t.on_allocate( p, s, a );
    /// \endcode
    ///
    /// \c T receives information indicating that \c p was allocated with size
    /// \c s and alignment \c a
    ///
    /// - - - - -
    ///
    /// \code
    /// t.on_deallocate( info, p, s );
    /// \endcode
    ///
    /// \c T receives information indicating that \c p was deallocated and size
    /// requested was \c s, for the allocator indicated by info \c i
    ///
    /// - - - - -
    ///
    /// \code
    /// t.on_deallocate_all();
    /// \endcode
    ///
    /// \c T receives information indicating the all memory was deallocated
    /// from within the allocator
    ///
    /// - - - - -
    ///
    /// \code
    /// t.finalize( i );
    /// \endcode
    ///
    /// \c T receives notification that all memory tracking has been completed.
    /// It is at this time that finalization of the tracking may occur (e.g.
    /// determining global memory leaks)
    ///
    ///////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 202000L
    // TODO(bitwize) replace 202000L with the correct __cplusplus when certified

    template<typename T>
    concept bool MemoryTracker = requires( T t,
                                           void* p,
                                           std::size_t s,
                                           std::size_t a,
                                           allocator_info i )
    {
      { t.on_allocate( p, s, a ) } -> void;
      { t.on_deallocate( i, p, s ) } -> void;
      { t.on_deallocate_all() } -> void;
      { t.finalize( i ) } -> void;
    };
#endif

    namespace detail {

      template<typename T, typename = void>
      struct memory_tracker_has_on_allocate : std::false_type{};

      template<typename T>
      struct memory_tracker_has_on_allocate<T,void_t<
        decltype( std::declval<T&>().on_allocate( std::declval<void*&>(), std::declval<std::size_t>(), std::declval<std::size_t>() ) )
      >> : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct memory_tracker_has_on_deallocate : std::false_type{};

      template<typename T>
      struct memory_tracker_has_on_deallocate<T,void_t<
        decltype( std::declval<T&>().on_deallocate( std::declval<allocator_info>(), std::declval<void*&>(), std::declval<std::size_t>() ) )
      >> : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct memory_tracker_has_on_deallocate_all : std::false_type{};

      template<typename T>
      struct memory_tracker_has_on_deallocate_all<T,void_t<
        decltype( std::declval<T&>().on_deallocate( std::declval<allocator_info>(), std::declval<void*&>(), std::declval<std::size_t>() ) )
      >> : std::true_type{};

      //-----------------------------------------------------------------------

      template<typename T, typename = void>
      struct memory_tracker_has_finalize : std::false_type{};

      template<typename T>
      struct memory_tracker_has_finalize<T,void_t<
        decltype( std::declval<T&>().finalize( std::declval<allocator_info>() ) )
      >> : std::true_type{};

    } // namespace detail

    /// \brief Type-trait determining whether \p T is a \c MemoryTracker
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam T the type to check
    template<typename T>
    struct is_memory_tracker : std::integral_constant<bool,
      detail::memory_tracker_has_on_allocate<T>::value &&
      detail::memory_tracker_has_on_deallocate<T>::value &&
      detail::memory_tracker_has_on_deallocate_all<T>::value &&
      detail::memory_tracker_has_finalize<T>::value &&
      std::is_default_constructible<T>::value
    >{};

    /// \brief Convenience template variable for extracting the result from
    ///        \c is_memory_tracker<T>::value
    ///
    /// \tparam T the type to check
    template<typename T>
    constexpr bool is_memory_tracker_v = is_memory_tracker<T>::value;

  } // namespace memory
} // namespace bit

#endif /* BIT_MEMORY_CONCEPTS_MEMORYTRACKER_HPP */
