#pragma once

#include <memory>
#include <array>
#include <map>
#include <format>

namespace otus_hw6{

    template<typename T, T def_value, size_t DIM_COUNT = 2>
    class Matrix : public std::enable_shared_from_this<Matrix<T, def_value, DIM_COUNT>>
    {
        using coord_t = std::array<size_t, DIM_COUNT>;
        using cont_t  = std::map<coord_t, T>;
    public:
        static constexpr const T empty_value = def_value;
        static constexpr const size_t dimension_count = DIM_COUNT;
        static constexpr const size_t null_index = static_cast<size_t>(-1);

        class DimProxy
        {
            friend class Matrix;
            using Matrix_Ptr_t = std::shared_ptr<Matrix>;
            DimProxy(Matrix_Ptr_t const& owner, const size_t dim_idx, const size_t cell_idx) 
                :   owner_(owner),
                    cell_index_(cell_idx)
            {
                set_dim_idx(dim_idx);
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
                coord_t ret;
                ret.fill(null_index);
                size_t i = 0;
                for(DimProxy const* p = this; p ; p = p->next_dim_.get())
                    ret[i++] = p->cell_index_;
                return ret;
            }

        public:
            /// @brief вернуть прокси для следующего измерения матрицы
            /// @param cell_idx индекс элемента в возвращаемом измерении 
            /// @return ссылку на объект следующего измерения, пока что в рантайме проверяем число измерений   
            DimProxy&  get_dim(size_t cell_idx)
            {
                if(!next_dim_)
                    next_dim_.reset( new DimProxy(owner_, dim_index_ + 1, cell_idx) );
                return *next_dim_.get();
            }

            /// @brief установить значение. Должно быть последним измерением матрицы, иначе std::range_error
            /// @param v 
            /// @return ссылку на себя, чтобы сделать присваивание по цепочке 
            DimProxy& set_value(const T& v)
            {
                auto k = get_val_key();
                owner_->set_val_by_key(k, v);
                return *this;
            }
            
            /// @brief вернуть значение ячейки. 
            /// @return значение хранимого типа. Если следующего измерения нет, или нет значения с полным ключом путей в хранилище - вернуть значение по умолчанию
            T         get_value() const
            {
                return DIM_COUNT - 1 == dim_index_ ? owner_->get_val_by_key(get_val_key()) : def_value;
            } 
        private:
            using DimProxy_Ptr_t = std::unique_ptr<DimProxy>;
            Matrix_Ptr_t owner_;

            size_t  dim_index_ = null_index;
            size_t  cell_index_ = null_index;
            DimProxy_Ptr_t next_dim_;                           
        };

        
        Matrix() {}
        
        /**
                 * @brief вернуть прокси для первого измерения с присвоением ему индекса элемента в этом измерении
                 * 
                 * @param element_idx 
                 * @return DimProxy 
                 */
        DimProxy get_dim(size_t element_idx) 
        {
            return DimProxy(this->shared_from_this(), DIM_COUNT, element_idx);
        }

        size_t size() const { return values_.size(); }
    private:
        T       get_val_by_key(const coord_t& key) const
        {
            auto p = values_.find(key);
            return values_.end() != p ? p->second : def_value;
        }

        void    set_val_by_key(const coord_t& key, const T& val)
        {
            if( std::end(key) == std::find(std::begin(key), std::end(key), null_index) )
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


} // otus_hw5

