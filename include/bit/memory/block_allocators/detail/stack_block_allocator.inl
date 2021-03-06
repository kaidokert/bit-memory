#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_STACK_BLOCK_ALLOCATOR_INL
#define BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_STACK_BLOCK_ALLOCATOR_INL

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align>
inline bit::memory::stack_block_allocator<BlockSize,Blocks,Align>
  ::stack_block_allocator()
  noexcept
{
  auto* s = static_cast<char*>(m_storage); // cast array to pointer
  for( auto i=0; i<Blocks; ++i ) {
    auto* p = static_cast<void*>(s + (i * BlockSize));

    m_cache.store_block( {p, BlockSize} );
  }
}

//-----------------------------------------------------------------------------
// Block Allocations
//-----------------------------------------------------------------------------

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align>
inline bit::memory::owner<bit::memory::memory_block>
  bit::memory::stack_block_allocator<BlockSize,Blocks,Align>::allocate_block()
  noexcept
{
  return m_cache.request_block();
}

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align>
inline void bit::memory::stack_block_allocator<BlockSize,Blocks,Align>
  ::deallocate_block( owner<memory_block> block )
  noexcept
{
  assert( block.data() >= static_cast<void*>(&m_storage[0]) );
  assert( block.data() < static_cast<void*>(&m_storage[s_storage_size]) );
  assert( block != nullblock );

  m_cache.store_block( block );
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align>
inline std::size_t bit::memory::stack_block_allocator<BlockSize,Blocks,Align>
  ::next_block_size()
  const noexcept
{
  if( m_cache.empty() ) return 0;
  return BlockSize;
}

template<std::size_t BlockSize, std::size_t Blocks, std::size_t Align>
inline bit::memory::allocator_info
  bit::memory::stack_block_allocator<BlockSize,Blocks,Align>::info()
  const noexcept
{
  return {"stack_block_allocator",this};
}

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_DETAIL_STACK_BLOCK_ALLOCATOR_INL */
