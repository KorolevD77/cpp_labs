#include <iostream>


/*

// К заданиям 1, 2

template <typename T>
class Grid final {
public:
    using value_type = T;
    using size_type = unsigned;

private:
    T* data;
    size_type y_size, x_size;

    class RowProxy {
    private:
        T* row_data;
        size_type row_size;
        
    public:
        RowProxy(T* row_data, size_type row_size) 
            : row_data(row_data), row_size(row_size) {}
            
        T& operator[](size_type x_idx) {
            if (x_idx >= row_size) {
                throw std::out_of_range("Column index out of range");
            }
            return row_data[x_idx];
        }
        
        const T& operator[](size_type x_idx) const {
            if (x_idx >= row_size) {
                throw std::out_of_range("Column index out of range");
            }
            return row_data[x_idx];
        }
    };

public:
    Grid(T const &t) : y_size(1), x_size(1) {
        data = new T[1];
        data[0] = t;
    }

    Grid(size_type y_size, size_type x_size) : y_size(y_size), x_size(x_size) {
        data = new T[y_size * x_size];
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = T();
        }
    }

    Grid(size_type y_size, size_type x_size, T const &t) : y_size(y_size), x_size(x_size) {
        data = new T[y_size * x_size];
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = t;
        }
    }

    ~Grid() {
        delete[] data;
    }

    Grid(Grid const &other) : y_size(other.y_size), x_size(other.x_size) {
        data = new T[y_size * x_size];
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = other.data[i];
        }
    }

    Grid& operator=(Grid const &other) {
        if (this != &other) {
            T* new_data = new T[other.y_size * other.x_size];
            for (size_type i = 0; i < other.y_size * other.x_size; ++i) {
                new_data[i] = other.data[i];
            }
            
            delete[] data;
            data = new_data;
            y_size = other.y_size;
            x_size = other.x_size;
        }
        return *this;
    }

    Grid(Grid&& other) noexcept 
        : data(other.data), y_size(other.y_size), x_size(other.x_size) {
        other.data = nullptr;
        other.y_size = 0;
        other.x_size = 0;
    }

    Grid& operator=(Grid&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            y_size = other.y_size;
            x_size = other.x_size;
            other.data = nullptr;
            other.y_size = 0;
            other.x_size = 0;
        }
        return *this;
    }

    RowProxy operator[](size_type y_idx) {
        if (y_idx >= y_size) {
            throw std::out_of_range("Row index out of range. Shit happens.");
        }
        return RowProxy(data + y_idx * x_size, x_size);
    }

    const RowProxy operator[](size_type y_idx) const {
        if (y_idx >= y_size) {
            throw std::out_of_range("Row index out of range. Shit happens.");
        }
        return RowProxy(data + y_idx * x_size, x_size);
    }

    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];
    }

    T& operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];
    }

    Grid& operator=(T const &t) {
        for (auto it = data, end = data + x_size * y_size; it != end; ++it) {
            *it = t;
        }
        return *this;
    }

    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }
};
*/



#include <utility>

template<typename T, size_t N>
class Grid;

template<typename T>
class Grid<T, 1> {
public:
    using value_type = T;
    using size_type = size_t;

private:
    T* data;
    size_type size;

public:
    Grid() : data(nullptr), size(0) {} 

    Grid(size_type size, const T& value = T()) : size(size) {
        data = new T[size];
        for (size_type i = 0; i < size; ++i) 
            data[i] = value;
        
    }

    ~Grid() { delete[] data; }

    Grid(const Grid& other) : size(other.size) {
        data = new T[size];
        for (size_type i = 0; i < size; ++i) 
            data[i] = other.data[i];
        
    }

    Grid(Grid&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }

    Grid& operator=(const Grid& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new T[size];
            for (size_type i = 0; i < size; ++i) 
                data[i] = other.data[i];
            
        }
        return *this;
    }

    Grid& operator=(Grid&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    T operator()(size_type index) const { return data[index]; }
    T& operator()(size_type index) { return data[index]; }
    T operator[](size_type index) const { return data[index]; }
    T& operator[](size_type index) { return data[index]; }

    size_type get_size() const { return size; }
};

template<typename T, size_t N>
class Grid {
public:
    using value_type = T;
    using size_type = size_t;

private:
    Grid<T, N-1>* data;
    size_type first_dim;

public:
    Grid() : data(nullptr), first_dim(0) {} 

    template<typename... Args>
    Grid(Args... args) {
        static_assert(sizeof...(args) == N + 1, "Wrong number of constructor arguments. Shit happens.");
        
        size_type all_dims[N + 1] = {static_cast<size_type>(args)...};
        first_dim = all_dims[0];
        T value = all_dims[N];
        
        data = new Grid<T, N-1>[first_dim];
        
        for (size_type i = 0; i < first_dim; ++i)
            data[i] = create_subgrid(std::make_index_sequence<N - 1>{}, all_dims, value);
        
    }

private:
    template<size_t... Is>
    Grid<T, N-1> create_subgrid(std::index_sequence<Is...>, const size_type* all_dims, const T& value) {
        return Grid<T, N-1>(all_dims[Is + 1]..., value);
    }

    template<size_t... Is>
    static decltype(auto) apply_to_subgrid(Grid<T, N-1>& subgrid, const size_type* indexes, std::index_sequence<Is...>) {
        return subgrid(indexes[Is + 1]...);
    }

public:
    ~Grid() { delete[] data; }

    Grid(const Grid& other) : first_dim(other.first_dim) {
        data = new Grid<T, N-1>[first_dim];
        for (size_type i = 0; i < first_dim; ++i)
            data[i] = other.data[i];
        
    }

    Grid(Grid&& other) noexcept : data(other.data), first_dim(other.first_dim) {
        other.data = nullptr;
        other.first_dim = 0;
    }

    Grid& operator=(const Grid& other) {
        if (this != &other) {
            delete[] data;
            first_dim = other.first_dim;
            data = new Grid<T, N-1>[first_dim];
            for (size_type i = 0; i < first_dim; ++i)
                data[i] = other.data[i];
            
        }
        return *this;
    }

    Grid& operator=(Grid&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            first_dim = other.first_dim;
            other.data = nullptr;
            other.first_dim = 0;
        }
        return *this;
    }

    template<typename... Args>
    T operator()(Args... args) const {
        static_assert(sizeof...(args) == N, "Wrong number of indexes. Shit happens.");
        
        size_type indexes[N] = {static_cast<size_type>(args)...};
        Grid<T, N-1>& subgrid = data[indexes[0]];
        return apply_to_subgrid(subgrid, indexes, std::make_index_sequence<N - 1>{});
    }

    template<typename... Args>
    T& operator()(Args... args) {
        static_assert(sizeof...(args) == N, "Wrong number of indexes. Shit happens.");
        
        size_type indexes[N] = {static_cast<size_type>(args)...};
        Grid<T, N-1>& subgrid = data[indexes[0]];
        return apply_to_subgrid(subgrid, indexes, std::make_index_sequence<N - 1>{});
    }

    Grid<T, N-1> operator[](size_type index) const {
        return data[index];
    }

    Grid<T, N-1>& operator[](size_type index) {
        return data[index];
    }

    size_type get_size() const { return first_dim; }
};


#include <cassert>

int main() {
Grid<float,3> const g3(2, 3, 4, 1.0f);
assert(1.0f == g3(1, 1, 1));

Grid<float,2> g2(2, 5, 2.0f);
assert(2.0f == g2(1, 1));

g2 = g3[1];
assert(1.0f == g2(1, 1));
}
