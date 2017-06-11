/**
 * \file alignment.hpp
 *
 * \brief todo: fill in documentation
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_ALIGNMENT_HPP
#define BIT_MEMORY_ALIGNMENT_HPP

#include "memory_block.hpp" // memory_block, nullblock

#include <cstdlib> // std::size_t
#include <utility> // std::forward
#include <cstdint> // std::uintpr_t

namespace bit {
  namespace memory {

    //------------------------------------------------------------------------
    // Nullability
    //------------------------------------------------------------------------

    /// \brief Checks whether a given pointer is null
    ///
    /// \param ptr the pointer to check
    /// \return \c true if the ptr is \c nullptr
    template<typename Ptr>
    constexpr auto is_null( Ptr&& ptr ) noexcept
      -> decltype(ptr==nullptr, bool());

    /// \brief Checks whether a given pointer is null
    ///
    /// This overload always returns \c true
    ///
    /// \return \c true
    constexpr bool is_null( std::nullptr_t ) noexcept;

    /// \brief Checks whether a given block is nullblock
    ///
    /// This overload always returns \c true
    ///
    /// \return \c true
    constexpr bool is_null( memory_block block ) noexcept;

    /// \brief Checks whether a given block is nullblock
    ///
    /// \param block the block to check
    /// \return \c true if the block is \c nullblock
    constexpr bool is_null( nullblock_t ) noexcept;

    //------------------------------------------------------------------------
    // Alignment Checking
    //------------------------------------------------------------------------

    /// \brief Determines whether a given alignment value is a valid alignment
    ///
    /// \param alignment the boundary to align to
    /// \return \c true if it's a valid alignment
    constexpr bool is_valid_alignment( std::size_t alignment ) noexcept;

    /// \brief Gets the alignment of the pointer
    ///
    /// \param ptr the pointer
    /// \return the alignment of the pointer
    std::size_t align_of( void* ptr ) noexcept;

    //------------------------------------------------------------------------
    //
    //------------------------------------------------------------------------

    std::size_t distance( const void* lhs, const void* rhs ) noexcept;

    std::ptrdiff_t difference( const void* lhs, const void* rhs ) noexcept;

    void* advance( void* lhs, std::ptrdiff_t bytes ) noexcept;

    //------------------------------------------------------------------------
    // Offset Calculation
    //------------------------------------------------------------------------

    /// \brief Calculates the forward-offset required to align \p ptr to
    ///        \p align alignment
    ///
    /// \param ptr The pointer to align
    /// \param align The alignment to calculate
    /// \param offset The amount of offset to leave prior to alignment
    /// \return the amount of bytes required to align to the next aligned
    ///         address
    /// \{
    std::size_t align_forward_offset( void* ptr,
                                      std::size_t align ) noexcept;
    std::size_t align_forward_offset( void* ptr,
                                      std::size_t align,
                                      std::size_t offset ) noexcept;
    /// \}

    /// \brief Calculates the backward-offset required to align \p ptr to
    ///        \p align alignment
    ///
    /// \param ptr The pointer to align
    /// \param align The alignment to calculate
    /// \param offset The amount of offset to leave prior to alignment
    /// \return the amount of bytes required to align to the next aligned
    ///         address
    /// \{
    std::size_t align_backward_offset( void* ptr,
                                       std::size_t align ) noexcept;
    std::size_t align_backward_offset( void* ptr,
                                       std::size_t align,
                                       std::size_t offset ) noexcept;
    /// \}

    //------------------------------------------------------------------------
    // Align
    //------------------------------------------------------------------------

    /// \brief Aligns the pointer \p ptr forward to the next
    ///        \p align byte address
    ///
    /// \param ptr
    /// \param size
    /// \param align
    /// \return
    void* align_forward( void* ptr,
                         std::size_t alignment,
                         std::size_t* adjust = nullptr ) noexcept;

    void* align_backward( void* ptr,
                          std::size_t alignment,
                          std::size_t* adjust = nullptr ) noexcept;

    /// \copydoc align_forward
    constexpr auto align = &align_forward;

    //------------------------------------------------------------------------
    // Align with Offset
    //------------------------------------------------------------------------

    void* offset_align_forward( void* ptr,
                                std::size_t alignment,
                                std::size_t offset,
                                std::size_t* adjust = nullptr ) noexcept;

    void* offset_align_backward( void* ptr,
                                 std::size_t alignment,
                                 std::size_t offset,
                                 std::size_t* adjust = nullptr ) noexcept;

    /// \copydoc offset_align_forward
    constexpr auto offset_align = &offset_align_forward;

  } // namespace memory
}  // namespace bit

template<typename Ptr>
inline constexpr auto bit::memory::is_null( Ptr&& ptr )
  noexcept -> decltype( ptr==nullptr, bool() )
{
  return ptr == nullptr;
}

inline constexpr bool bit::memory::is_null( std::nullptr_t )
  noexcept
{
  return true;
}

inline constexpr bool bit::memory::is_null( memory_block block )
  noexcept
{
  return block == nullblock;
}

inline constexpr bool bit::memory::is_null( nullblock_t )
  noexcept
{
  return true;
}

//----------------------------------------------------------------------------
// Alignment Checking
//----------------------------------------------------------------------------

inline constexpr bool bit::memory::is_valid_alignment( std::size_t alignment )
  noexcept
{
  auto c = 0;
  do {
    if( alignment & 1 ) ++c;
    if( c > 1 ) return false;

  } while( alignment >>= 1 );

  return true;
}

inline std::size_t bit::memory::align_of( void* ptr )
  noexcept
{
  auto address = reinterpret_cast<std::uintptr_t>(ptr);
  auto align   = std::size_t{0};

  while( !(address & 1) )
  {
    address >>= 1;
    ++align;
  }

  return (1u << align);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

inline std::size_t bit::memory::distance( const void* lhs, const void* rhs )
  noexcept
{
  return (lhs > rhs) ? difference(lhs,rhs) : difference(rhs,lhs);
}

inline std::ptrdiff_t bit::memory::difference( const void* lhs, const void* rhs )
  noexcept
{
  return static_cast<const char*>(lhs) - static_cast<const char*>(rhs);
}

inline void* bit::memory::advance( void* lhs, std::ptrdiff_t bytes )
  noexcept
{
  return static_cast<char*>(lhs) + bytes;
}

//----------------------------------------------------------------------------
// Align
//----------------------------------------------------------------------------

inline void* bit::memory::align_forward( void* ptr,
                                         std::size_t alignment,
                                         std::size_t* adjust )
  noexcept
{
  const auto address = reinterpret_cast<std::uintptr_t>(ptr);
  void* new_ptr      = reinterpret_cast<void*>( ((address + static_cast<std::uintptr_t>(alignment-1)) & static_cast<std::uintptr_t>(~(alignment-1))) );
  if( adjust ) {
    *adjust = static_cast<std::size_t>( reinterpret_cast<std::uintptr_t>(new_ptr) - address );
  }
  return new_ptr;
}

//----------------------------------------------------------------------------

inline void* bit::memory::align_backward( void* ptr,
                                          std::size_t alignment,
                                          std::size_t* adjust )
  noexcept
{
  const auto address = reinterpret_cast<std::uintptr_t>(ptr);
  void* new_ptr      = reinterpret_cast<void*>(((address) & (~static_cast<std::uintptr_t>(alignment-1))));
  if( adjust ) {
    *adjust = static_cast<std::size_t>(address & static_cast<std::uintptr_t>(alignment-1));
  }
  return new_ptr;
}

//----------------------------------------------------------------------------
// Align with Offset
//----------------------------------------------------------------------------

inline void* bit::memory::offset_align_forward( void* ptr,
                                                std::size_t alignment,
                                                std::size_t offset,
                                                std::size_t* adjust )
  noexcept
{
  const auto address = reinterpret_cast<std::uintptr_t>(ptr);
  void* new_ptr      = reinterpret_cast<void*>( ((address + offset + static_cast<std::uintptr_t>(alignment-1)) & static_cast<std::uintptr_t>(~(alignment-1))) - offset );
  if( adjust ) {
    *adjust = static_cast<std::size_t>( reinterpret_cast<std::uintptr_t>(new_ptr) - address );
  }
  return new_ptr;
}

//----------------------------------------------------------------------------

inline void* bit::memory::offset_align_backward( void* ptr,
                                                 std::size_t alignment,
                                                 std::size_t offset,
                                                 std::size_t* adjust )
  noexcept
{
  const auto address = reinterpret_cast<std::uintptr_t>(ptr);
  void* new_ptr      = reinterpret_cast<void*>(((address) & (~static_cast<std::uintptr_t>(alignment-1))) - offset);
  if( adjust ) {
    *adjust = static_cast<std::size_t>(address & static_cast<std::uintptr_t>(alignment-1));
  }
  return new_ptr;
}

#endif /* BIT_MEMORY_ALIGNMENT_HPP */
