/**
 * \file dynamic_size_type.hpp
 *
 * \brief This internal header contains an EBO-optimized type for storing
 *        possibly compile-time size values.
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
 */
#ifndef BIT_MEMORY_DETAIL_DYNAMIC_SIZE_TYPE_HPP
#define BIT_MEMORY_DETAIL_DYNAMIC_SIZE_TYPE_HPP

#include <cstddef>     // std::size_t
#include <type_traits> // std::true_type, std::false_type

namespace bit {
  namespace memory {
    namespace detail {

      constexpr std::size_t dynamic_size = std::size_t(-1);

      /////////////////////////////////////////////////////////////////////////
      /// \brief This type is used as a compile-time constant for storing
      ///        size values
      ///
      /// If \p Size is dynamic_size, then this type holds a data member.
      ///
      /// \tparam Index the index of the value (used for unique identification
      ///               in ebo)
      /// \tparam Size the size value
      /////////////////////////////////////////////////////////////////////////
      template<std::size_t Index, std::size_t Size>
      class dynamic_size_type
      {
        //---------------------------------------------------------------------
        // Public Member Types
        //---------------------------------------------------------------------
      public:

        using is_stateless = std::true_type;

        //---------------------------------------------------------------------
        // Constructors / Assignment
        //---------------------------------------------------------------------
      public:

        /// \brief Default-constructs the dynamic_size_type
        constexpr dynamic_size_type() noexcept = default;

        /// \brief Move-constructs a dynamic_size_type from another one
        ///
        /// \param other the other dynamic_size_type to move
        dynamic_size_type( dynamic_size_type&& other ) noexcept = default;

        /// \brief Copy-constructs a dynamic_size_type from another one
        ///
        /// \param other the other dynamic_size_type to copy
        dynamic_size_type( const dynamic_size_type& other ) noexcept = default;

        //---------------------------------------------------------------------

        /// \brief Move-assigns a dynamic_size_type from another one
        ///
        /// \param other the other dynamic_size_type to move
        /// \return reference to \c (*this)
        dynamic_size_type& operator=( dynamic_size_type&& other ) noexcept = default;

        /// \brief Copy-assigns a dynamic_size_type from another one
        ///
        /// \param other the other dynamic_size_type to copy
        /// \return reference to \c (*this)
        dynamic_size_type& operator=( const dynamic_size_type& other ) noexcept = default;

        //---------------------------------------------------------------------
        // Observers
        //---------------------------------------------------------------------
      public:

        /// \brief Retrieves the value
        ///
        /// \return Size
        constexpr std::size_t value() const noexcept;
      };

      //-----------------------------------------------------------------------

      template<std::size_t Index>
      class dynamic_size_type<Index,dynamic_size>
      {
        //---------------------------------------------------------------------
        // Public Member Types
        //---------------------------------------------------------------------
      public:

        using is_stateless = std::false_type;

        //---------------------------------------------------------------------
        // Constructors / Assignment
        //---------------------------------------------------------------------
      public:

        /// \brief Constructs a dynamic_size_type from a given \p size
        ///
        /// \param size the size to store
        constexpr explicit dynamic_size_type( std::size_t size ) noexcept;

        /// \brief Move-constructs a dynamic_size_type from another one
        ///
        /// \param other the other dynamic_size_type to move
        dynamic_size_type( dynamic_size_type&& other ) noexcept = default;

        /// \brief Copy-constructs a dynamic_size_type from another one
        ///
        /// \param other the other dynamic_size_type to copy
        dynamic_size_type( const dynamic_size_type& other ) noexcept = default;

        //---------------------------------------------------------------------

        /// \brief Move-assigns a dynamic_size_type from another one
        ///
        /// \param other the other dynamic_size_type to move
        /// \return reference to \c (*this)
        dynamic_size_type& operator=( dynamic_size_type&& other ) noexcept = default;

        /// \brief Copy-assigns a dynamic_size_type from another one
        ///
        /// \param other the other dynamic_size_type to copy
        /// \return reference to \c (*this)
        dynamic_size_type& operator=( const dynamic_size_type& other ) noexcept = default;

        //---------------------------------------------------------------------
        // Observers
        //---------------------------------------------------------------------
      public:

        /// \brief Retrieves the stored size
        ///
        /// \return the stored size
        constexpr std::size_t value() const noexcept;

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        std::size_t m_size;
      };

    } // namespace detail
  } // namespace memory
} // namespace bit

//=============================================================================
// dynamic_size_type<Index,Size>
//=============================================================================

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<std::size_t Index, std::size_t Size>
inline constexpr std::size_t bit::memory::detail::dynamic_size_type<Index,Size>
  ::value()
  const noexcept
{
  return Size;
}

//=============================================================================
// dynamic_size_type<Index,dynamic_size>
//=============================================================================

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

template<std::size_t Index>
inline constexpr bit::memory::detail::dynamic_size_type<Index,bit::memory::detail::dynamic_size>
  ::dynamic_size_type( std::size_t size )
  noexcept
  : m_size(size)
{

}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<std::size_t Index>
inline constexpr std::size_t bit::memory::detail::dynamic_size_type<Index,bit::memory::detail::dynamic_size>
  ::value()
  const noexcept
{
  return m_size;
}


#endif /* BIT_MEMORY_DETAIL_DYNAMIC_SIZE_TYPE_HPP */