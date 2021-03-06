/*****************************************************************************
 * \file
 * \brief This header contains definitions for various debugging utilities
 *****************************************************************************/

/*
  The MIT License (MIT)

  Copyright (c) 2018 Matthew Rodusek

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifndef BIT_MEMORY_UTILITIES_DEBUGGING_HPP
#define BIT_MEMORY_UTILITIES_DEBUGGING_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "memory_block.hpp" // memory_block

#include <cstddef>   // std::size_t

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief This enumeration contains all tag byte marks that are used for
    ///        debugging allocations.
    //////////////////////////////////////////////////////////////////////////
    enum class debug_tag : unsigned char
    {
        allocated_block_byte = 0xAB, //!< Memory allocated in a block, but not
                                     //!< from an allocator
        freed_block_byte     = 0xFB, //!< Memory freed
        allocated_byte       = 0xCD, //!< Memory allocated by an allocator
        freed_byte           = 0xDD, //!< Memory freed by an allocator
        padding_byte         = 0xED, //!< byte mark for generated padding for
                                     //!< alignments or offsets
        fence_start_byte     = 0x19, //!< byte mark for low-order addresses to
                                     //!< determine memory stomps
        fence_end_byte       = 0x91, //!< byte mark for high-order addresses to
                                     //!< determine memory stomps
    };

    /// \{
    /// \brief Tags memory with the \ref debug_tag bytes
    ///
    /// \param p pointer to the memory to tag
    /// \param n the number of bytes to tag
    /// \param tag the tag to write
    void debug_tag_bytes( void* p, std::size_t n, debug_tag tag );
    void debug_tag_block_allocated_bytes( void* p, std::size_t n );
    void debug_tag_block_freed_bytes( void* p, std::size_t n );
    void debug_tag_fence_start_bytes( void* p, std::size_t n );
    void debug_tag_fence_end_bytes( void* p, std::size_t n );
    void debug_tag_allocated_bytes( void* p, std::size_t n );
    void debug_tag_freed_bytes( void* p, std::size_t n );
    /// \}


    /// \{
    /// \brief Untags memory previously tagged with \ref debug_tag bytes
    ///
    /// This function validates that the bytes previously tagged still contain
    /// the same tag (thus no buffer-overflow conditions have occurred).
    ///
    /// If a buffer overflow has occurred, then this function returns a pointer
    /// to the first byte that has been overwritten, and \p stomped will now
    /// contain the number of bytes that have been modified.
    ///
    /// Otherwise this function returns \c nullptr, and \c stomped remains
    /// unchanged
    ///
    /// \param p pointer to the memory to untag
    /// \param n the number of bytes to untag
    /// \param tag the expected debug tag
    /// \param [out] stomped outputs the number of bytes stomped if a buffer
    ///              overflow occurs; otherwise remains unchanged
    /// \return pointer to the first byte that does not match \p tag (if
    ///         modified); otherwise returns nullptr
    void* debug_untag_bytes( void* p, std::size_t n, debug_tag tag, std::size_t* stomped );
    void* debug_untag_fence_start_bytes( void* p, std::size_t n, std::size_t* stomped );
    void* debug_untag_fence_end_bytes( void* p, std::size_t n, std::size_t* stomped );
    /// \}

  } // namespace memory
} // namespace bit

#include "detail/debugging.inl"

#endif /* BIT_MEMORY_UTILITIES_DEBUGGING_HPP */
