module;

#include <macro.hpp>

export module gal.image:pixmap;

import std;
import gal.utility;
import gal.memory;

#if defined(G_COMPILER_MSVC)
	#define ARR_SUBSCRIPT_OPERATOR_WORKAROUND_LEFT operator(
	#define ARR_SUBSCRIPT_OPERATOR_WORKAROUND_RIGHT )
#else
	#define ARR_SUBSCRIPT_OPERATOR_WORKAROUND_LEFT operator[
	#define ARR_SUBSCRIPT_OPERATOR_WORKAROUND_RIGHT ]
#endif

#define ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR ARR_SUBSCRIPT_OPERATOR_WORKAROUND_LEFT ARR_SUBSCRIPT_OPERATOR_WORKAROUND_RIGHT

export namespace gal::gui::image
{
	template<typename T, typename Allocator = memory::AnyAllocator<T>>
	class Pixmap;

	template<typename T>
	class PixmapView;
}// namespace gal::gui::image

namespace gal::gui::image
{
	template<typename T>
	using phantom = std::span<T>;

	template<typename P>
	class RowView : public std::ranges::view_interface<RowView<P>>
	{
	public:
		constexpr static bool is_const = std::is_const_v<P>;

		using value_type			   = typename P::value_type;
		using size_type				   = typename P::size_type;
		using difference_type		   = typename P::difference_type;
		using row_type				   = typename P::row_type;
		using const_row_type		   = typename P::const_row_type;

	private:
		class Sentinel
		{
			template<bool>
			friend class Iterator;

		public:
			using size_type = size_type;

		private:
			size_type total_y_;

		public:
			constexpr explicit Sentinel(const size_type total_y) noexcept
				: total_y_{total_y} {}
		};

		template<bool ConstIterator>
		class Iterator
		{
		public:
			using value_type		= std::conditional_t<ConstIterator, const_row_type, row_type>;
			using size_type			= size_type;
			using difference_type	= difference_type;
			using iterator_concept	= std::bidirectional_iterator_tag;
			using iterator_category = std::bidirectional_iterator_tag;

		private:
			P&		  pv_;
			size_type current_y_;

		public:
			constexpr Iterator(P& pv, const size_type y)
				: pv_{pv},
				  current_y_{y} {}

			constexpr auto operator++() noexcept -> Iterator&
			{
				++current_y_;
				return *this;
			}

			constexpr auto operator++(int) noexcept -> Iterator
			{
				Iterator tmp{*this};
				++current_y_;
				return tmp;
			}

			constexpr auto operator--() noexcept -> Iterator&
			{
				--current_y_;
				return *this;
			}

			constexpr auto operator--(int) noexcept -> Iterator
			{
				Iterator tmp{*this};
				--current_y_;
				return tmp;
			}

			[[nodiscard]] constexpr auto operator*() const noexcept -> decltype(auto)
			{
				return pv_[current_y_];
			}

			[[nodiscard]] constexpr auto operator->() const noexcept -> decltype(auto)
			{
				return &pv_[current_y_];
			}

			[[nodiscard]] constexpr auto operator==(const Sentinel& sentinel) const noexcept -> bool
			{
				return current_y_ == sentinel.total_y_;
			}

			[[nodiscard]] friend constexpr auto iter_move(const Iterator& it) noexcept -> decltype(auto)
				requires(not ConstIterator)
			{
				return std::move(*it);
			}

			friend constexpr auto iter_swap(const Iterator& lhs, const Iterator& rhs) noexcept -> void
				requires(not ConstIterator)
			{
				using std::swap;

				swap(*lhs, *rhs);
			}
		};

		P& pv_;

	public:
		constexpr explicit RowView(P& pv) noexcept
			: pv_{pv} {}

		[[nodiscard]] constexpr auto begin() noexcept -> Iterator<is_const>
		{
			return {pv_, static_cast<size_type>(0)};
		}

		[[nodiscard]] constexpr auto end() noexcept -> Sentinel
		{
			return Sentinel{pv_.height()};
		}
	};

	template<typename T>
	RowView(Pixmap<T>& pv) -> RowView<Pixmap<T>>;

	template<typename T>
	RowView(const Pixmap<T>& pv) -> RowView<const Pixmap<T>>;

	template<typename T>
	RowView(PixmapView<T>& pv) -> RowView<PixmapView<T>>;

	template<typename T>
	RowView(const PixmapView<T>& pv) -> RowView<const PixmapView<T>>;

	export
	{
		/**
		 * @brief A non-owning 2D pixel-based image.
		 * @tparam T The pixel format.
		 */
		template<typename T>
		class PixmapView
		{
			using phantom = phantom<T>;

		public:
			using value_type			 = phantom::value_type;
			using size_type				 = phantom::size_type;
			using difference_type		 = phantom::difference_type;
			using pointer				 = phantom::pointer;
			using const_pointer			 = phantom::const_pointer;
			using reference				 = phantom::reference;
			using const_reference		 = phantom::const_reference;
			using iterator				 = phantom::iterator;
			using reverse_iterator		 = phantom::reverse_iterator;
			using const_iterator		 = phantom::const_iterator;
			using const_reverse_iterator = phantom::const_reverse_iterator;

			using row_type				 = std::span<value_type>;
			using const_row_type		 = std::span<const value_type>;

		private:
			pointer						 data_;
			size_type					 width_;
			size_type					 height_;
			size_type					 stride_;

			[[nodiscard]] constexpr auto as_phantom() const noexcept -> phantom
			{
				return {data_, size()};
			}

		public:
			constexpr PixmapView(const PixmapView&) noexcept					= default;
			constexpr PixmapView(PixmapView&&) noexcept							= default;
			constexpr auto operator=(const PixmapView&) noexcept -> PixmapView& = default;
			constexpr auto operator=(PixmapView&&) noexcept -> PixmapView&		= default;
			constexpr ~PixmapView() noexcept									= default;

			constexpr PixmapView() noexcept
				: data_{nullptr},
				  width_{0},
				  height_{0},
				  stride_{0} {}

			constexpr PixmapView(
					pointer	  data,
					size_type width,
					size_type height,
					size_type stride) noexcept
				: data_{data},
				  width_{width},
				  height_{height},
				  stride_{stride} {}

			constexpr PixmapView(
					pointer	  data,
					size_type width,
					size_type height) noexcept
				: PixmapView{data, width, height, width} {}

			template<std::same_as<std::remove_const_t<value_type>> U, typename Allocator>
			constexpr explicit(false) PixmapView(// NOLINT
					const Pixmap<U, Allocator>& pixmap) noexcept
				: PixmapView{pixmap.data(), pixmap.width(), pixmap.height()}
			{
			}

			template<std::same_as<std::remove_const_t<value_type>> U, typename Allocator>
			constexpr explicit(false) PixmapView(// NOLINT
					Pixmap<U, Allocator>& pixmap) noexcept
				: PixmapView{pixmap.data(), pixmap.width(), pixmap.height()}
			{
			}

			template<std::same_as<std::remove_const_t<value_type>> U, typename Allocator>
			constexpr explicit(false) PixmapView(// NOLINT
					Pixmap<U, Allocator>&& pixmap) noexcept = delete;

			[[nodiscard]] constexpr explicit(false) operator PixmapView<const value_type>() const noexcept// NOLINT
				requires(not std::is_const_v<value_type>)
			{
				return {
						data_,
						width_,
						height_,
						stride_};
			}

			[[nodiscard]] constexpr auto empty() const noexcept -> bool
			{
				return width_ == 0 and height_ == 0;
			}

			[[nodiscard]] constexpr explicit operator bool() const noexcept
			{
				return not empty();
			}

			[[nodiscard]] constexpr auto width() const noexcept -> size_type
			{
				return width_;
			}

			[[nodiscard]] constexpr auto height() const noexcept -> size_type
			{
				return height_;
			}

			[[nodiscard]] constexpr auto stride() const noexcept -> size_type
			{
				return stride_;
			}

			[[nodiscard]] constexpr auto size() const noexcept -> size_type
			{
				return width_ * height_;
			}

			// D:\a\_work\1\s\src\vctools\Compiler\CxxFE\sl\p1\c\module\writer.cpp:7847: sorry: not yet implemented
			// fatal error C1001: ICE
			// template<typename Self>
			// [[nodiscard]] constexpr auto data(this Self&& self) noexcept -> std::conditional_t<std::is_const_v<Self>, const_pointer, pointer>
			// {
			// 	return std::forward<Self>(self).data_;
			// }

			[[nodiscard]] constexpr auto data() noexcept -> pointer
			{
				return data_;
			}

			[[nodiscard]] constexpr auto data() const noexcept -> const_pointer
			{
				return data_;
			}

			[[nodiscard]] constexpr auto begin() noexcept -> iterator
			{
				return as_phantom().begin();
			}

			[[nodiscard]] constexpr auto begin() const noexcept -> const_iterator
			{
				return as_phantom().begin();
			}

			[[nodiscard]] constexpr auto end() noexcept -> iterator
			{
				return as_phantom().end();
			}

			[[nodiscard]] constexpr auto end() const noexcept -> const_iterator
			{
				return as_phantom().end();
			}

			[[nodiscard]] constexpr auto operator[](const size_type y) noexcept -> row_type
			{
				GAL_GUI_ASSUME(y < height_);

				return {data_ + y * stride_, width_};
			}

			[[nodiscard]] constexpr auto operator[](const size_type y) const noexcept -> const_row_type
			{
				GAL_GUI_ASSUME(y < height_);

				return {data_ + y * stride_, width_};
			}

			[[nodiscard]] constexpr auto ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(const size_type x, const size_type y) noexcept -> reference
			{
				GAL_GUI_ASSUME(x < width_ and y < height_);

				return data_[x + y * stride_];
			}

			[[nodiscard]] constexpr auto ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(const size_type x, const size_type y) const noexcept -> const_reference
			{
				GAL_GUI_ASSUME(x < width_ and y < height_);

				return data_[x + y * stride_];
			}

			[[nodiscard]] constexpr auto rows() noexcept -> auto
			{
				return RowView{*this};
			}

			[[nodiscard]] constexpr auto rows() const noexcept -> auto
			{
				return RowView{*this};
			}

			[[nodiscard]] constexpr auto sub_view(const size_type begin_x, const size_type begin_y, const size_type new_width, const size_type new_height) noexcept -> PixmapView<value_type>
			{
				auto* new_data = &this->ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(begin_x, begin_y);
				return PixmapView<value_type>{
						new_data,
						new_width,
						new_height,
						stride_};
			}

			[[nodiscard]] constexpr auto sub_view(const size_type begin_x, const size_type begin_y, const size_type new_width, const size_type new_height) const noexcept -> PixmapView<const value_type>
			{
				const auto* new_data = &this->ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(begin_x, begin_y);
				return PixmapView<const value_type>{
						new_data,
						new_width,
						new_height,
						stride_};
			}

			friend constexpr auto copy(PixmapView source, PixmapView<std::remove_const_t<value_type>> dest) noexcept -> void// NOLINT
			{
				GAL_GUI_ASSUME(source.width() == dest.width(), "Width mismatch!");
				GAL_GUI_ASSUME(source.height() == dest.height(), "Height mismatch!");

				if (source.stride() == source.width() and source.stride() == dest.stride())
				{
					std::ranges::copy(source, dest.begin());
				}
				else
				{
					for (auto& [source_row, dest_row]: std::views::zip(source.rows(), dest.rows()))
					{
						std::ranges::copy(source_row, dest_row.begin());
					}
				}
			}

			friend constexpr auto fill(PixmapView dest, const value_type value = value_type{}) noexcept
			{
				if (dest.stride() == dest.width())
				{
					std::ranges::fill(dest, value);
				}
				else
				{
					for (auto& row: dest.rows())
					{
						std::ranges::fill(row, value);
					}
				}
			}
		};

		template<typename T, typename Allocator>
		PixmapView(Pixmap<T, Allocator> & pixmap) -> PixmapView<T>;

		template<typename T, typename Allocator>
		PixmapView(const Pixmap<T, Allocator>& pixmap) -> PixmapView<const T>;
	}

	export
	{
		/**
		 * @brief A 2D pixel-based image.
		 * @tparam T The pixel format.
		 * @tparam Allocator The allocator to use for allocating the array of pixels.
		 */
		template<typename T, typename Allocator>
		class Pixmap
		{
			using phantom = phantom<T>;

		public:
			using allocator_type		 = Allocator;
			using allocator_traits_type	 = std::allocator_traits<allocator_type>;

			using value_type			 = phantom::value_type;
			using size_type				 = phantom::size_type;
			using difference_type		 = phantom::difference_type;
			using pointer				 = phantom::pointer;
			using const_pointer			 = phantom::const_pointer;
			using reference				 = phantom::reference;
			using const_reference		 = phantom::const_reference;
			using iterator				 = phantom::iterator;
			using reverse_iterator		 = phantom::reverse_iterator;
			using const_iterator		 = phantom::const_iterator;
			using const_reverse_iterator = phantom::const_reverse_iterator;

			using row_type				 = std::span<value_type>;
			using const_row_type		 = std::span<const value_type>;

		private:
			pointer									 data_;
			size_type								 width_;
			size_type								 height_;
			size_type								 capacity_;
			GAL_GUI_NO_UNIQUE_ADDRESS allocator_type allocator_;

			[[nodiscard]] constexpr auto			 as_phantom() const noexcept -> phantom
			{
				return {data_, size()};
			}

		public:
			constexpr Pixmap(const Pixmap& other) noexcept(false)
				: data_{nullptr},
				  width_{other.width()},
				  height_{other.height()},
				  capacity_{other.size()},
				  allocator_{allocator_traits_type::select_on_container_copy_construction(other.allocator_)}
			{
				data_ = allocator_traits_type::allocate(allocator_, capacity_);
				std::ranges::uninitialized_copy(other, *this);
			}

			constexpr Pixmap(Pixmap&& other) noexcept
				: data_{std::exchange(other.data_, nullptr)},
				  width_{std::exchange(other.width_, 0)},
				  height_{std::exchange(other.height_, 0)},
				  capacity_{std::exchange(other.capacity_, 0)},
				  allocator_{std::exchange(other.allocator_, {})} {}

			constexpr auto operator=(const Pixmap& other) noexcept(false) -> Pixmap&
			{
				if (this == &other)
				{
					return *this;
				}

				constexpr auto propagate_allocator = allocator_traits_type::propagate_on_container_copy_assignment::value;

				const auto	   replace_allocator   = allocator_ != other.allocator_ and propagate_allocator;

				if (capacity_ >= other.size() and not replace_allocator)
				{
					static_assert(std::is_nothrow_copy_constructible_v<value_type>);

					clear();
					width_	= other.width_;
					height_ = other.height_;
					std::ranges::uninitialized_copy(other, *this);
					return *this;
				}

				auto&	   new_allocator = replace_allocator ? const_cast<allocator_type&>(other.allocator_) : allocator_;
				const auto new_capacity	 = other.size();

				pointer	   new_data		 = nullptr;
				try
				{
					new_data = allocator_traits_type::allocate(new_allocator, new_capacity);
					std::ranges::uninitialized_copy(other, new_data, new_data + new_capacity);
				}
				catch (...)
				{
					allocator_traits_type::deallocate(allocator_, new_data, new_capacity);
					throw;
				}

				clear();
				try
				{
					shrink_to_fit();
				}
				catch (...)
				{
					std::ranges::destroy_n(new_data, new_capacity);
					allocator_traits_type::deallocate(allocator_, new_data, new_capacity);
					throw;
				}

				data_	   = new_data;
				width_	   = other.width_;
				height_	   = other.height_;
				capacity_  = new_capacity;
				allocator_ = new_allocator;
				return *this;
			}

			constexpr auto operator=(Pixmap&& other) noexcept(false) -> Pixmap&
			{
				if (this == &other)
				{
					return *this;
				}

				constexpr auto propagate_allocator = allocator_traits_type::propagate_on_container_copy_assignment::value;

				if (allocator_ == other.allocator_ or not propagate_allocator)
				{
					clear();
					shrink_to_fit();

					data_	   = std::exchange(other.data_, nullptr);
					width_	   = std::exchange(other.width_, 0);
					height_	   = std::exchange(other.height_, 0);
					capacity_  = std::exchange(other.capacity_, 0);
					allocator_ = std::exchange(other.allocator_, {});
					return *this;
				}

				if (capacity_ >= other.size())
				{
					clear();

					width_	= other.width_;
					height_ = other.height_;

					std::ranges::uninitialized_move(other, *this);

					// Clear, but leave the allocation intact, so that it can be reused.
					other.clear();
					return *this;
				}

				const auto new_capacity = other.size();

				pointer	   new_data		= nullptr;
				try
				{
					new_data = allocator_traits_type::allocate(allocator_, new_capacity);
					std::ranges::uninitialized_move(other, new_data, new_data + new_capacity);
				}
				catch (...)
				{
					allocator_traits_type::deallocate(allocator_, new_data, new_capacity);
					throw;
				}

				clear();
				try
				{
					shrink_to_fit();
				}
				catch (...)
				{
					std::ranges::destroy_n(new_data, new_capacity);
					allocator_traits_type::deallocate(allocator_, new_data, new_capacity);
					throw;
				}

				data_	  = new_data;
				width_	  = other.width_;
				height_	  = other.height_;
				capacity_ = new_capacity;

				// Clear, but leave the allocation intact, so that it can be reused.
				other.clear();
				return *this;
			}

			constexpr ~Pixmap() noexcept
			{
				std::ranges::destroy(*this);
				if (data_)
				{
					allocator_traits_type::deallocate(allocator_, data_, capacity_);
				}
			}

			constexpr Pixmap() noexcept
				: data_{nullptr},
				  width_{0},
				  height_{0},
				  capacity_{0},
				  allocator_{} {}

			constexpr Pixmap(
					const size_type width,
					const size_type height,
					allocator_type	allocator = allocator_type{}) noexcept(false)
				: data_{allocator_traits_type::allocate(allocator, width * height)},
				  width_{width},
				  height_{height},
				  capacity_{width_ * height_},
				  allocator_{allocator}
			{
				std::ranges::uninitialized_value_construct(*this);
			}

			template<std::convertible_to<value_type> U>
			constexpr Pixmap(
					const U*		source,
					const size_type width,
					const size_type height,
					const size_type stride,
					allocator_type	allocator = allocator_type{}) noexcept(false)
				: data_{allocator_traits_type::allocate(allocator, width * height)},
				  width_{width},
				  height_{height},
				  capacity_{width_ * height_},
				  allocator_{allocator}
			{
				GAL_GUI_NOT_NULL(source);

				if (width_ == stride)
				{
					try
					{
						std::ranges::uninitialized_copy(source, source + capacity_, data_, data_ + capacity_);
					}
					catch (...)
					{
						allocator_traits_type::deallocate(allocator_, data_, capacity_);
						throw;
					}
				}
				else
				{
					auto it_source	 = source;
					auto it_dest	 = data_;
					auto it_dest_end = data_ + capacity_;

					try
					{
						while (it_dest != it_dest_end)
						{
							std::ranges::uninitialized_copy(it_source, it_source + width, it_dest);
							it_source += stride;
							it_dest += width;
						}
					}
					catch (...)
					{
						std::ranges::destroy(data_, it_dest);
						allocator_traits_type::deallocate(allocator_, data_, capacity_);
						throw;
					}
				}
			}

			template<std::convertible_to<value_type> U>
			constexpr Pixmap(
					const U*		source,
					const size_type width,
					const size_type height,
					allocator_type	allocator = allocator_type{}) noexcept(false)
				: Pixmap{source, width, height, width, allocator}
			{
			}

			template<std::convertible_to<value_type> U>
			constexpr explicit Pixmap(
					const PixmapView<const U> other,
					allocator_type			  allocator = allocator_type{}) noexcept(false)
				: Pixmap{other.data(), other.width(), other.height(), allocator}
			{
			}

			template<std::convertible_to<value_type> U, typename A>
			constexpr explicit Pixmap(
					const Pixmap<U, A>& other,
					allocator_type		allocator = allocator_type{}) noexcept(false)
				: Pixmap{other.operator PixmapView<const U>(), allocator}
			{
			}

			[[nodiscard]] constexpr explicit(false) operator PixmapView<value_type>() const noexcept// NOLINT
			{
				return {data_, width_, height_};
			}

			[[nodiscard]] constexpr explicit(false) operator PixmapView<const value_type>() const noexcept// NOLINT
			{
				return {data_, width_, height_};
			}

			[[nodiscard]] constexpr auto operator==(const Pixmap& other) const noexcept
			{
				if (width() != other.width() or height() != other.height())
				{
					return false;
				}

				return std::ranges::equal(other, *this);
			}

			[[nodiscard]] constexpr auto get_allocator() const noexcept -> allocator_type
			{
				return allocator_;
			}

			[[nodiscard]] constexpr auto empty() const noexcept -> bool
			{
				return width() == 0 and height() == 0;
			}

			[[nodiscard]] constexpr explicit operator bool() const noexcept
			{
				return not empty();
			}

			[[nodiscard]] constexpr auto width() const noexcept -> size_type
			{
				return width_;
			}

			[[nodiscard]] constexpr auto height() const noexcept -> size_type
			{
				return height_;
			}

			/**
			 * @return The number of pixels (width * height) in this image.
			 */
			[[nodiscard]] constexpr auto size() const noexcept -> size_type
			{
				return width_ * height_;
			}

			/**
			 * @return The number of pixels of capacity allocated.
			 */
			[[nodiscard]] constexpr auto capacity() const noexcept -> size_type
			{
				return capacity_;
			}

			[[nodiscard]] constexpr auto data() noexcept -> pointer
			{
				return data_;
			}

			[[nodiscard]] constexpr auto data() const noexcept -> const_pointer
			{
				return data_;
			}

			[[nodiscard]] constexpr auto begin() noexcept -> iterator
			{
				return as_phantom().begin();
			}

			[[nodiscard]] constexpr auto begin() const noexcept -> const_iterator
			{
				return as_phantom().begin();
			}

			[[nodiscard]] constexpr auto end() noexcept -> iterator
			{
				return as_phantom().end();
			}

			[[nodiscard]] constexpr auto end() const noexcept -> const_iterator
			{
				return as_phantom().end();
			}

			[[nodiscard]] constexpr auto operator[](const size_type y) noexcept -> row_type
			{
				GAL_GUI_ASSUME(y < height_);

				return {data_ + y * width_, width_};
			}

			[[nodiscard]] constexpr auto operator[](const size_type y) const noexcept -> const_row_type
			{
				GAL_GUI_ASSUME(y < height_);

				return {data_ + y * width_, width_};
			}

			[[nodiscard]] constexpr auto ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(const size_type x, const size_type y) noexcept -> reference
			{
				GAL_GUI_ASSUME(x < width_ and y < height_);

				return data_[x + y * width_];
			}

			[[nodiscard]] constexpr auto ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(const size_type x, const size_type y) const noexcept -> const_reference
			{
				GAL_GUI_ASSUME(x < width_ and y < height_);

				return data_[x + y * width_];
			}

			[[nodiscard]] constexpr auto rows() noexcept -> auto
			{
				return RowView{*this};
			}

			[[nodiscard]] constexpr auto rows() const noexcept -> auto
			{
				return RowView{*this};
			}

			[[nodiscard]] constexpr auto sub_view(const size_type begin_x, const size_type begin_y, const size_type new_width, const size_type new_height) noexcept -> PixmapView<const value_type>
			{
				return operator PixmapView<value_type>().sub_view(begin_x, begin_y, new_width, new_height);
			}

			[[nodiscard]] constexpr auto sub_pixmap(const size_type begin_x, const size_type begin_y, const size_type new_width, const size_type new_height, allocator_type allocator) noexcept(false) -> Pixmap
			{
				GAL_GUI_ASSUME(begin_x + new_width <= width_ and begin_y + new_height <= height_);

				const auto* new_data = &this->ARR_SUBSCRIPT_OPERATOR_WORKAROUND_OPERATOR(begin_x, begin_y);
				return {
						new_data,
						new_width,
						new_height,
						allocator};
			}

			[[nodiscard]] constexpr auto sub_pixmap(const size_type begin_x, const size_type begin_y, const size_type new_width, const size_type new_height) noexcept(false) -> Pixmap
			{
				return sub_pixmap(begin_x, begin_y, new_width, new_height, allocator_);
			}

			constexpr auto clear() noexcept -> void
			{
				std::ranges::destroy(*this);
				width_	= 0;
				height_ = 0;
			}

			constexpr auto shrink_to_fit() noexcept(false) -> void
			{
				if (empty())
				{
					if (data_)
					{
						allocator_traits_type::deallocate(allocator_, data_, capacity_);
						data_	  = nullptr;
						capacity_ = 0;
					}
					return;
				}

				const auto new_capacity = size();

				pointer	   new_data		= nullptr;
				try
				{
					new_data = allocator_traits_type::allocate(allocator_, new_capacity);
					std::ranges::uninitialized_move(begin(), end(), new_data, new_data + new_capacity);
				}
				catch (...)
				{
					allocator_traits_type::deallocate(allocator_, new_data, new_capacity);
					throw;
				}

				std::ranges::destroy(*this);
				const auto old_data		= std::exchange(data_, new_data);
				const auto old_capacity = std::exchange(capacity_, new_capacity);
				allocator_traits_type::deallocate(allocator_, old_data, old_capacity);
			}

			friend constexpr auto fill(Pixmap& dest, const value_type value = value_type{}) noexcept
			{
				std::ranges::fill(dest, value);
			}
		};

		template<typename T, typename Allocator = memory::AnyAllocator<std::remove_const_t<T>>>
		Pixmap(PixmapView<T> pv, Allocator allocator = Allocator{}) -> Pixmap<std::remove_const_t<T>, Allocator>;
	}
}// namespace gal::gui::image
