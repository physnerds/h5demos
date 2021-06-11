#ifndef H5_WRITE_1D_CHARS_H
#define H5_WRITE_1D_CHARS_H

#include "utilities.h"

using namespace HighFive;
using product_t = std::vector<char>;

template<typename T>
void CreateOffsetDataSet(Group lumi,T val, unsigned long batch,std::string offset_name){
  DataSetCreateProps props;
  props.add(Chunking(std::vector<hsize_t>{batch}));
  DataSpace dataspace = DataSpace({batch},{DataSpace::UNLIMITED});
  DataSet dataset = lumi.createDataSet<int>(offset_name,dataspace,props);
  dataset.write(val);
}

std::vector<char>
flatten(std::vector<std::vector<char>> const& tmp) {
  std::vector<char> tmp1 = tmp[0];
  for (int i = 1; i<tmp.size(); ++i)
     tmp1.insert(end(tmp1), begin(tmp[i]), end(tmp[i]));
  return tmp1;
}

void
write_1D_chars(std::vector<product_t> const& products, 
               std::vector<std::string> const& ds_names, 
               long unsigned int batch, 
               long unsigned int nbatch, 
               long unsigned int round,
               Group lumi) 
{
  auto num_prods = ds_names.size();
  //  std::cout<<"Size of the product "<<num_prods<<" "<<products.size()<<std::endl;
  for (int prod_index = 0; prod_index < num_prods; ++prod_index) {
    auto prod_size = products[prod_index].size(); 
    auto tmp = get_prods(products, prod_index, num_prods);
    auto sizes = get_sizes(tmp);
    auto tmp1 = flatten(tmp);
    auto sizes1 = get_sizes1D(tmp1);
    auto ds_name = ds_names[prod_index];
    //   auto sum_prods = std::accumulate(sizes.begin(), sizes.end(), 0);
    auto sum_prods = std::accumulate(sizes.begin(),sizes.end(),0);
    //create and extend dataset for sizes here!
    auto szds_name = ds_name+"_sz";
    auto offset_name = ds_name+"_offset";
    if(!lumi.exist(szds_name)) {
        std::cout << "creating size dataset for : " << ds_name <<" "<<batch<< '\n';
      DataSetCreateProps props;
      props.add(Chunking(std::vector<hsize_t>{batch}));
      DataSpace dataspace = DataSpace({batch}, {DataSpace::UNLIMITED});
      DataSet dataset = lumi.createDataSet<int>(szds_name, dataspace, props);
      // dataset.write(sizes);
     
      dataset.write(sizes);
    } else {
      DataSet dataset = lumi.getDataSet(szds_name); 
      //    std::cout << "extending size dataset for : " << dataset.getPath() << "\n";     
      auto offset = dataset.getDimensions()[0];//batch*round;
      dataset.resize({offset+batch});
      // dataset.select({offset}, {nbatch}).write(sizes);
      dataset.select({offset},{batch}).write(sizes);
    }
    
    //now write data products
    if(!lumi.exist(ds_names[prod_index])){ //if dataset doesn't exist, create it otherwise extend it
      if(!lumi.exist(offset_name))CreateOffsetDataSet(lumi,0,batch,offset_name);
         std::cout << "creating dataset for: " << ds_name << " "<<sum_prods<<"\n";
      DataSetCreateProps props;
      props.add(Chunking(std::vector<hsize_t>{batch*sum_prods}));
      DataSpace dataspace = DataSpace({batch*sum_prods}, {DataSpace::UNLIMITED});
      DataSet dataset = lumi.createDataSet<char>(ds_name, dataspace, props);
      dataset.write(tmp1);
      }
    else {
      DataSet dataset = lumi.getDataSet(ds_names[prod_index]);
      DataSet dataoffset = lumi.getDataSet(offset_name);
      auto d_offset = dataoffset.getDimensions()[0];
      dataoffset.resize({d_offset+batch});
      //    std::cout << "extending dataset for: " << dataset.getPath() << "\n";
      //  auto offset = batch*round*sum_prods;
      //Use what is used in the root_serialization
        auto offset = dataset.getDimensions()[0];
	//  std::cout<<"Offset "<<offset<<std::endl;
	//auto new_dimension = offset+sum_prods;
	  auto new_dimension = offset+sum_prods;
      dataset.resize({new_dimension}); //resize to the new dimension.....
      //offset is the old dimension of the data-set
      // std::cout<<offset<<" "<<nbatch<<" "<<sum_prods<<" "<<tmp1.size()<<" "<<round<<" "<<offset+batch*sum_prods<<std::endl;
      dataset.select({offset}, {tmp1.size()}).write(tmp1); //get_prods(products, prod_index, num_prods));
      // dataoffset.select({d_offset},{nbatch}).write(offset);
  }
  }
}

void write_2D_chars(std::vector<product_t> const& products, 
               std::vector<std::string> const& ds_names, 
               long unsigned int batch, 
               long unsigned int nbatch, 
               long unsigned int round,
               Group lumi) 
{
  auto num_prods = ds_names.size();
  auto noffset = 0.0;
  for (int prod_index = 0; prod_index < num_prods; ++prod_index) {
    auto prod_size = products[prod_index].size(); 
    auto tmp = get_prods(products, prod_index, num_prods);
    auto sizes = get_sizes(tmp);
    auto tmp1 = flatten(tmp);
    auto ds_name = ds_names[prod_index];
    auto sum_prods = std::accumulate(sizes.begin(), sizes.end(), 0);
    //create and extend dataset for sizes here!
    auto szds_name = ds_name+"_sz";
    if(!lumi.exist(szds_name)) {
      std::cout << "creating size dataset for : " << ds_name << '\n';
      DataSetCreateProps props;
      props.add(Chunking(std::vector<hsize_t>{batch}));
      std::cout<<"Done Chunking "<<std::endl;
      DataSpace dataspace = DataSpace({batch}, {DataSpace::UNLIMITED});
      std::cout<<"Done Creating DataSpace "<<std::endl;
      DataSet dataset = lumi.createDataSet<int>(szds_name, dataspace, props);
      std::cout<<"Done Creating DataSet "<<std::endl;

      dataset.write(sizes);
      std::cout<<"Done Writing "<<std::endl;
    } else {
      DataSet dataset = lumi.getDataSet(szds_name); 
      // std::cout << "extending size dataset for : " << dataset.getPath() << "\n";     
      auto offset = batch*round;
      dataset.resize({offset+batch});
      dataset.select({offset}, {nbatch}).write(sizes);
    }
    //now write data products
    if(!lumi.exist(ds_names[prod_index])){ //if dataset doesn't exist, create it otherwise extend it
      std::cout << "creating dataset for: " << ds_name << "\n";
      DataSetCreateProps props;
      props.add(Chunking(std::vector<hsize_t>{batch*sum_prods}));
      DataSpace dataspace = DataSpace({batch*sum_prods}, {DataSpace::UNLIMITED});
      DataSet dataset = lumi.createDataSet<char>(ds_name, dataspace, props); 
      dataset.write(tmp);
      }
    else {
      DataSet dataset = lumi.getDataSet(ds_names[prod_index]);
      std::cout << "extending dataset for: " << dataset.getPath() << "\n";
      auto offset = batch*round*sum_prods;
      dataset.resize({offset+(batch*sum_prods)}); 
    dataset.select({offset}, {nbatch*sum_prods}).write(tmp); //get_prods(products, prod_index, num_prods));
    }
  }
}

#endif
