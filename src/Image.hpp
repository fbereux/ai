/*******************************************************
 * Copyright (c) François Béreux 2015 - All Rights Reserved
 * 
 * This file is part of Hi-BOX library
 * It is proprietary and confidential
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 *
 * Written by François Béreux <francois.bereux@gmail.com>
 *
 *******************************************************/

#include <iostream>
#include <cassert>
#include <fstream>

/**
 @brief MNIST image format
*/ 
class Idx {
private:
  int n_images_;
  int n_rows_;
  int n_cols_;
  float *data_;

private:
  // some utilities

  // convert a high-endian (Idx) to little-endian (Intel)
  static int reverse_int( int i ) {
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return((int) ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
  }

public:

  Idx( const std::string filename ) {
    std::ifstream file(filename, std::ios::binary);
    assert( file.is_open() );
    // read header
    int magic_number;
    file.read((char*) &magic_number, sizeof(magic_number));
    magic_number = reverse_int(magic_number);
    assert( magic_number == 2051 );
    file.read((char*) &n_images_,sizeof(n_images_));
    n_images_ = reverse_int(n_images_);
    file.read((char*) &n_rows_, sizeof(n_rows_));
    n_rows_ = reverse_int(n_rows_);
    file.read((char*) &n_cols_, sizeof(n_cols_));
    n_cols_ = reverse_int(n_cols_);
    // allocate data_
    int image_size = n_rows_*n_cols_;
    data_ = new float[ image_size*n_images_ ];
    // read images
    unsigned char temp;
    for(int i = 0; i < n_images_; ++i)
    {
      for(int r = 0; r < n_rows_; ++r)
      {
        for(int c = 0; c < n_cols_; ++c)
        {
          file.read((char*) &temp, sizeof(temp));
          data_[ i*image_size + c*n_rows_ + r ] = temp;
        }
      }
    }
  }
  
  int n_images( ) const { return n_images_; };
  int n_rows( ) const { return n_rows_; };
  int n_cols( ) const { return n_cols_; };

  float* image( int num ) const {
    return &data_[num*n_rows_*n_cols_];
  }

  ~Idx() {
    delete[] data_;
  }

  static int* read_labels( const std::string filename, int& n_images ) {
    std::ifstream file (filename, std::ios::binary);
    assert( file.is_open() );
    // read header
    int magic_number;
    file.read((char*) &magic_number, sizeof(magic_number));
    magic_number = reverse_int(magic_number);
    assert( magic_number == 2049 );
    file.read((char*) &n_images,sizeof(n_images));
    n_images = reverse_int(n_images);
    // allocate labels
    int *labels = new int[ n_images ];
    // read labels
    unsigned char temp;
    for(int i = 0; i < n_images; ++i)
    {
      file.read((char*) &temp, sizeof(temp));
      labels[i] = temp;
    }
    return labels;
  }
  
}; // class Idx

/**
 @brief (restriction of) Portable Grey Map (PGM) image format
*/
class Pgm {
private:
  const static int maxval = 255; // enforced to conform to Idx
  int width_;
  int height_;
  char* data_;

public:

  Pgm( const Idx& idx, const int num ) {
    width_         = idx.n_cols();
    height_        = idx.n_rows();
    int image_size = width_*height_;
    data_          = new char[ width_*height_ ];
    // copy and convert from Idx (where white = 0) to PGM (where white = 255)
    float* img     = idx.image( num );
    for(int p=0; p < image_size; ++p) {
      data_[p] = static_cast<char>( maxval - img[p] );
    }
  }

  void write( const std::string filename ) {
    std::ofstream file (filename, std::ios::binary);
    assert( file.is_open() );
    // write header
    char magic_number[3] = "P5"; // size is 3 because of final \000
    file.write( magic_number, 2 );
    char nl = '\n';
    file.write( &nl, 1 );
    file.write( reinterpret_cast<const char*>(&width_), sizeof( int ) );
    file.write( &nl, 1 );
    file.write( reinterpret_cast<const char*>(&height_), sizeof( int ) );
    file.write( &nl, 1 );
    file.write( reinterpret_cast<const char*>(&maxval), sizeof( int ) );
    file.write( &nl, 1 );
    // write image
    int image_size = width_*height_;
    file.write( data_, image_size );

    file.close();
  }

  ~Pgm() {
    delete [] data_;
  }

}; // class Pgm
