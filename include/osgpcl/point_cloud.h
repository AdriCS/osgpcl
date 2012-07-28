/*
 * point_cloud.h
 *
 *  Created on: Jun 23, 2012
 *      Author: asher
 */

#ifndef POINT_CLOUD_H_
#define POINT_CLOUD_H_

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include <osg/Geometry>
#include <osg/Uniform>

#include <boost/any.hpp>
#include <string>
#include <map>
#include <pcl/common/io.h>
namespace osgPCL
{

  /*
   * The point cloud visualization must be initialized with how it should be rendered
   * and then the point cloud should be set.
   */


  typedef osg::Geometry PointCloudGeometry;

  class PointCloudFactory {
    public:
      PointCloudFactory(){}
      virtual ~PointCloudFactory(){}

      virtual PointCloudGeometry* buildGeometry(bool unique_stateset=false) const =0;
    private:
      std::map<std::string, boost::any> input_clouds_;

    public:
      template<typename PointT>
      void setInputCloud(const typename pcl::PointCloud<PointT>::ConstPtr& cloud ){
        input_clouds_[pcl::getFieldsList(*cloud)] = cloud;
      }

    protected:
      osg::ref_ptr<osg::StateSet> stateset_;

      template<typename PointT>
      typename pcl::PointCloud<PointT>::ConstPtr getInputCloud() const ;

      template<typename PointT>
      void addXYZToVertexBuffer( osg::Geometry&, const pcl::PointCloud<pcl::PointXYZ>& cloud) const;
  };


  template<typename PointT=pcl::PointXYZ>
  class PointCloudColoredFactory : public PointCloudFactory {

    public:

    PointCloudColoredFactory();

    virtual PointCloudGeometry* buildGeometry(bool unique_stateset=false) const;


      void setColor(float r, float g, float b, float alpha =1);

    private:
  };


  template<typename PointTXYZ=pcl::PointXYZ, typename PointTF=pcl::PointXYZ>
  class PointCloudCRangeFactory : public PointCloudFactory {


    public:
      typedef boost::shared_ptr<typename pcl::PointCloud<PointTXYZ>::ConstPtr > CloudConstPtr;

    void setField(std::string field);
    void setRangle(double min, double max);
    void setColorTable( const std::vector<osg::Vec4>& table);

    void useJETColorTable();
    void useGreyColorTable();

    virtual PointCloudGeometry* buildGeometry(bool unique_stateset=false) const;

    void setPointSize(int size);

    protected:
      std::string field_name_;
      double min_range_, max_range_;
      std::vector<osg::Vec4> color_table_;
  };

  template<typename PointTXYZ=pcl::PointXYZ, typename RGBT=pcl::RGB>
  class PointCloudRGBFactory : public PointCloudFactory {

    public:
    virtual PointCloudGeometry* buildGeometry(bool unique_stateset=false) const;

  };


  template<typename PointTXYZ, typename IntensityT>
  class PointCloudIFactory : public PointCloudFactory {

    virtual PointCloudGeometry* buildGeometry() const;

  };


}

/* namespace osgPCL */
#endif /* POINT_CLOUD_H_ */
