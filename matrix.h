#pragma once

#include <memory>
#include <array>
#include <map>
#include <algorithm>

namespace otus_hw6{


    /// @brief Шаблон класса разреженной матрицы
    /// @tparam T - тип элементов
    /// @tparam def_value  - значение по умолчанию (null value)
    /// @tparam DIM_COUNT - число измерений 
    template<typename T, T def_value, size_t DIM_COUNT = 2>
    class Matrix 
    {
    public:
        using coord_t = std::array<size_t, DIM_COUNT>;
        using cont_t  = std::map<coord_t, T>;

        static const T empty_value;
        static const size_t dimension_count;
        static const size_t null_index;

        /// @brief информация о ячейке 
        ///        TODO - сделать автогенерацию tuple<size_t..., T> для этого типа  
        struct cell_info_t
        {
            coord_t coord_;
            T       val_;
            cell_info_t(coord_t const& coord, const T& val) : coord_(coord), val_(val) {}            
        };
        
        /// @brief простой однонаправленный итератор для обхода ячеек 
        class iterator 
        {
            friend class Matrix;
            typename cont_t::iterator p_;
            iterator(typename cont_t::iterator p) : p_(p) {}
        public:

            using iterator_category = std::forward_iterator_tag;
            using value_type = cell_info_t;
            using difference_type = std::ptrdiff_t;
            using pointer = cell_info_t*;
            using reference = cell_info_t&;        

            iterator& operator++(){ ++p_; return *this; }
            cell_info_t operator*(){ return cell_info_t{p_->first, p_->second}; }
            bool operator==(iterator& rhs) { return p_ == rhs.p_;  }
            bool operator!=(iterator& rhs) { return !(*this == rhs);  }
        };

        iterator begin(){ return iterator(values_.begin()); }
        iterator end(){ return iterator(values_.end()); }

        /// @brief Прокси для работы с ячейкой или измерением матрицы
        ///        Хранит ссылку на матрицу, которая породила прокси, ОСТОРОЖНО - голая ссылка, не weak_ptr
        class DimProxy
        {
            friend class Matrix;
        public:
            DimProxy const& operator[](size_t i) const
            {
                return const_cast<DimProxy&>(*this).get_dim(i);
            }

            DimProxy& operator[](size_t i)
            {
                return get_dim(i);
            }


            DimProxy& operator=(const T& val)
            {
                return set_value(val);
            }

            operator T() const
            {
                return get_value();
            }

        private:
            DimProxy(Matrix& owner, const size_t dim_idx, const size_t cell_idx, const coord_t& full_cell_idx) 
                :   owner_(owner),
                    full_cell_index_(full_cell_idx)
            {
                set_dim_idx(dim_idx);
                full_cell_index_[dim_idx] = cell_idx;
            }

            void check_is_cell_proxy() const
            {
                if( dim_index_ < (DIM_COUNT - 1) )
                    throw std::range_error("Element is not cell");
            }

            DimProxy& set_dim_idx(size_t dim_idx)
            {
                if( dim_idx >= DIM_COUNT )
                    throw std::range_error( "Dimension index must not be greater than " + std::to_string(DIM_COUNT - 1) );

                if( dim_index_ != dim_idx )
                {
                    next_dim_.reset(nullptr);
                    dim_index_ = dim_idx;
                }
                return *this;
            }

            coord_t get_val_key() const
            {
                return full_cell_index_;
            }

            /// @brief вернуть прокси для следующего измерения матрицы
            /// @param cell_idx индекс элемента в возвращаемом измерении 
            /// @return ссылку на объект следующего измерения, пока что в рантайме проверяем число измерений   
            DimProxy&  get_dim(size_t cell_idx)
            {
                if(!next_dim_)
                    next_dim_.reset( new DimProxy(owner_, dim_index_ + 1, cell_idx, full_cell_index_) );
                return *next_dim_.get();
            }

            /// @brief установить значение. Должно быть последним измерением матрицы, иначе std::range_error
            /// @param v 
            /// @return ссылку на себя, чтобы сделать присваивание по цепочке 
            DimProxy& set_value(const T& v)
            {
                check_is_cell_proxy();
                auto k = get_val_key();
                owner_.set_val_by_key(k, v);
                return *this;
            }
            
            /// @brief вернуть значение ячейки. 
            /// @return значение хранимого типа. Если следующего измерения нет, или нет значения с полным ключом путей в хранилище - вернуть значение по умолчанию
            T         get_value() const
            {
                return DIM_COUNT - 1 == dim_index_ ? owner_.get_val_by_key(get_val_key()) : def_value;
            } 

            using DimProxy_Ptr_t = std::unique_ptr<DimProxy>;
            Matrix& owner_;
            
            coord_t full_cell_index_;
            size_t  dim_index_ = null_index;
            DimProxy_Ptr_t next_dim_;                            
        };

        DimProxy const operator[](size_t i) const
        {
            return const_cast<Matrix*>(this)->get_dim(i);
        }

        DimProxy operator[](size_t i)
        {
            return get_dim(i);
        }

        size_t size() const { return values_.size(); }
    private:
        /// @brief вернуть прокси для первого измерения с присвоением ему индекса элемента в этом измерении 
        /// @param element_idx 
        /// @return DimProxy 
        DimProxy get_dim(size_t element_idx) 
        {
            coord_t full_index;
            full_index.fill(null_index);

            return DimProxy(*this, 0, element_idx, full_index);
        }


        /// @brief вернуть значение по координате ячекйи
        /// @param key  - DIM_COUNT-мерная координата
        /// @return значение хранимого типа 
        T       get_val_by_key(const coord_t& key) const
        {
            auto p = values_.find(key);
            return values_.end() != p ? p->second : def_value;
        }

        /// @brief  Установить значение по координате 
        /// @param key  - DIM_COUNT-мерная координата
        /// @param val  значение хранимого типа, если равно def_value - удалятся ячейка из контейнера
        void    set_val_by_key(const coord_t& key, const T& val)
        {
            if( std::end(key) != std::find(std::begin(key), std::end(key), null_index) )
                return;

            if( def_value != val )
            {
                values_[key] = val;
                return;
            }

            values_.erase(key);
        }

        cont_t  values_;
    };
    //---------------------------------------------------------------------------------
    template<typename T, T def_value, size_t DIM_COUNT>
    const T Matrix<T, def_value, DIM_COUNT>::empty_value = def_value;
    template<typename T, T def_value, size_t DIM_COUNT>
    const size_t Matrix<T, def_value, DIM_COUNT>::dimension_count = DIM_COUNT;
    template<typename T, T def_value, size_t DIM_COUNT>
    const size_t Matrix<T, def_value, DIM_COUNT>::null_index = static_cast<size_t>(-1);

} // otus_hw5

