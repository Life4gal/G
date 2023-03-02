module;

#include <gc.h>

#include <memory>
#include <type_traits>

export module gal.memory;

namespace gal::gui::memory
{
	auto allocate(const std::size_t size) -> void* { return GC_malloc(size); }

	auto allocate_without_pointer(const std::size_t size) -> void* { return GC_malloc_atomic(size); }

	auto allocate_without_collect(const std::size_t size) -> void* { return GC_malloc_uncollectable(size); }

	auto allocate_without_collect_and_pointer(const std::size_t size) -> void* { return GC_malloc_atomic_uncollectable(size); }

	auto reallocate(void* old_object, const std::size_t required_size) -> void* { return GC_realloc(old_object, required_size); }

	auto deallocate(void* data) -> void
	{
		GC_free(data);
	}

	export
	{
		template<typename T>
		struct can_allocate_atomic : std::false_type
		{
		};

		template<typename T>
			requires std::is_arithmetic_v<T>
		struct can_allocate_atomic<T> : std::true_type
		{
		};

		template<typename T>
		constexpr bool can_allocate_atomic_v = can_allocate_atomic<T>::value;

		template<typename T>
		class AnyAllocator
		{
		public:
			using value_type							 = T;
			using pointer								 = value_type*;
			using const_pointer							 = const value_type*;
			using void_pointer							 = void*;
			using const_void_pointer					 = const void*;

			using size_type								 = size_t;
			using difference_type						 = ptrdiff_t;

			using propagate_on_container_copy_assignment = std::false_type;
			using propagate_on_container_move_assignment = std::false_type;
			using propagate_on_container_swap			 = std::false_type;
			using is_always_equal						 = std::true_type;

			template<typename U>
			using rebind_alloc = AnyAllocator<U>;

			template<typename U>
			using rebind_traits														= std::allocator_traits<rebind_alloc<U>>;

			constexpr AnyAllocator() noexcept										= default;
			constexpr AnyAllocator(const AnyAllocator&) noexcept					= default;
			constexpr AnyAllocator(AnyAllocator&&) noexcept							= default;
			constexpr auto operator=(const AnyAllocator&) noexcept -> AnyAllocator& = default;
			constexpr auto operator=(AnyAllocator&&) noexcept -> AnyAllocator&		= default;
			constexpr ~AnyAllocator() noexcept										= default;

			template<typename U>
			constexpr explicit(false) AnyAllocator(const AnyAllocator<U>&) noexcept// NOLINT
			{
			}

			[[nodiscard]] constexpr auto allocate(const size_type size) -> pointer
			{
				(void)this;

				static_assert(sizeof(value_type), "value_type must be complete before calling allocate.");

				const auto allocate_size = size * sizeof(value_type);

				if constexpr (can_allocate_atomic_v<value_type>) { return static_cast<pointer>(memory::allocate_without_pointer(allocate_size)); }
				else { return static_cast<pointer>(memory::allocate(allocate_size)); }
			}

			constexpr auto deallocate(pointer pointer, const size_type size) noexcept -> void
			{
				(void)this;
				(void)size;

				memory::deallocate(pointer);
			}

			[[nodiscard]] friend constexpr auto operator==(const AnyAllocator&, const AnyAllocator&) noexcept -> bool { return true; }
		};

		template<typename T>
		class StlAllocator
		{
		public:
			using value_type							 = T;
			using pointer								 = value_type*;
			using const_pointer							 = const value_type*;
			using void_pointer							 = void*;
			using const_void_pointer					 = const void*;

			using size_type								 = size_t;
			using difference_type						 = ptrdiff_t;

			using propagate_on_container_copy_assignment = std::false_type;
			using propagate_on_container_move_assignment = std::false_type;
			using propagate_on_container_swap			 = std::false_type;
			using is_always_equal						 = std::true_type;

			template<typename U>
			using rebind_alloc = AnyAllocator<U>;

			template<typename U>
			using rebind_traits														= std::allocator_traits<rebind_alloc<U>>;

			constexpr StlAllocator() noexcept										= default;
			constexpr StlAllocator(const StlAllocator&) noexcept					= default;
			constexpr StlAllocator(StlAllocator&&) noexcept							= default;
			constexpr auto operator=(const StlAllocator&) noexcept -> StlAllocator& = default;
			constexpr auto operator=(StlAllocator&&) noexcept -> StlAllocator&		= default;
			constexpr ~StlAllocator() noexcept										= default;

			template<typename U>
			constexpr explicit(false) StlAllocator(const StlAllocator<U>&) noexcept// NOLINT
			{
			}

			[[nodiscard]] constexpr auto allocate(const size_type size) -> pointer
			{
				(void)this;

				static_assert(sizeof(value_type), "value_type must be complete before calling allocate.");

				const auto allocate_size = size * sizeof(value_type);

				if constexpr (can_allocate_atomic_v<value_type>) { return static_cast<pointer>(memory::allocate_without_collect_and_pointer(allocate_size)); }
				else { return static_cast<pointer>(memory::allocate_without_collect(allocate_size)); }
			}

			constexpr auto deallocate(pointer pointer, const size_type size) noexcept -> void
			{
				(void)this;
				(void)size;

				memory::deallocate(pointer);
			}

			[[nodiscard]] friend constexpr auto operator==(const StlAllocator&, const StlAllocator&) noexcept -> bool { return true; }
		};

		class TrivialAllocator
		{
		public:
			using value_type	= void;
			using size_type		= std::size_t;

			using pointer		= void*;
			using const_pointer = const void*;

			template<typename T>
			[[nodiscard]] auto allocate(const size_type size) const -> pointer
			{
				(void)this;

				static_assert(sizeof(T), "value_type must be complete before calling allocate.");

				const auto allocate_size = size * sizeof(T);

				if constexpr (can_allocate_atomic_v<T>) { return static_cast<pointer>(memory::allocate_without_pointer(allocate_size)); }
				else { return static_cast<pointer>(memory::allocate(allocate_size)); }
			}

			auto deallocate(pointer pointer, const size_type size) const noexcept -> void
			{
				(void)this;
				(void)size;

				memory::deallocate(pointer);
			}
		};
	}
}// namespace gal::gui::memory
