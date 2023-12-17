#ifndef MATRIX_H
#define MATRIX_H

namespace lazyDungeon::math {

template<typename T>
struct matrix{

    size_t rows;
    size_t cols;
    std::vector<T> mat;

    matrix() = default;
    matrix(const matrix&) = default;

    matrix(size_t r, size_t c):rows(r), cols(c){
        mat.resize(r*c);
    };

    matrix(size_t r, size_t c, T fill):rows(r), cols(c){
        mat.resize(r*c);
        std::fill(mat.begin(), mat.end(),fill);
    };

    inline T operator()(size_t row, size_t col) const{
        assert(((row * cols)+col) < mat.size() && "matrix out of bounds row col");
        return mat[(row * cols)+col];
    };

    inline T &operator()(size_t row, size_t col) {
        assert(((row * cols)+col) < mat.size() && "matrix out of bounds row col");
        return mat[(row * cols)+col];
    };

    inline T &operator[](size_t index) {
        assert((index) < mat.size() && "matrix out of bounds row col");
        return mat[index];
    };

    inline void insert(const matrix&in, size_t row, size_t col){
        assert(((row * cols)+col) < mat.size() && "matrix out of bounds row col");

        for(int r = 0; r < in.rows; r++){
            for(int c = 0; c < in.cols; c++){
                operator()(row+r,col+c) = in(r,c);
            }
        }
    }

    inline bool checkBounds(size_t row, size_t col) const{
        if(((row * cols)+col) < mat.size()){
            return true;
        }

        return false;
    }


    inline void multiply(const math::matrix<T>&other){
        assert(mat.size() == other.mat.size() && "matrix with different size (multiplication)");

        for (size_t i = 0; i < mat.size(); ++i) {
            mat[i] *= other.mat[i];
        }
    }

    inline void printMat(){
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                std::cout << "\t" << static_cast<int>(operator()(r,c)) << ",";
            }
            std::cout << "\n";
        }
        std::cout << "----------------------\n";
    }
};

}

#endif // MATRIX_H
