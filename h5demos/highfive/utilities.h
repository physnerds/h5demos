#ifndef UTILITIES_H
#define UTILITIES_H


using namespace HighFive;
using product_t = std::vector<char>;


std::vector<product_t> 
get_prods(std::vector<product_t> const & input, 
         int prod_index, 
         int stride) {
  std::vector<product_t> tmp; 
  for(int j = prod_index; j< input.size(); j+=stride) {
    //  std::cout<<"utilities.h "<<prod_index<<" "<<input.size()<<" "<<j<<" "<<stride<<std::endl;
    tmp.push_back(input[j]);
  }
  return tmp;
}

std::vector<size_t>
get_sizes(std::vector<product_t> const & prods) {
  std::vector<size_t> sizes;
  for(auto p: prods) 
    sizes.push_back(p.size());
  return sizes;
}

std::vector<size_t>
get_sizes1D(std::vector<char> const & prods){
  std::vector<size_t>sizes;
  sizes.push_back(prods.size());
  return sizes;

}
#endif
