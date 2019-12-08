#/**
*  @copyright  Copyright (c) 2019 Institute of Cyber-Systems & Control
*                            Zhejiang University
*                            All rights reserved
*
*  @file     viewer.cpp
*  @version  1.0.0
*  @author   Dr. Zhao Yongsheng
*            Email: zhaoyongsheng@zju.edu.cn
*            Blog: www.zhaoyongsheng.com
*  @date     2019/02/14 20:29:49
*  @details       
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>
#include <pcl/exceptions.h>

#/**
*  @fn           printUsage
*  @details      This function print the usage commands 
*  @param[in]    progName, the execution name
*/
static void printUsage (const char* progName)
{
  std::cout << "\n\nUsage: "<<progName<<" [options]\n\n"
            << "Options:\n"
            << "-------------------------------------------\n"
            << "-h           this help\n"
            << "-f           Visualize the PCD files listed in the input file\n"
            << "-p           Visualize the PCD file in the input path\n"
            << "-d           Visualize the PCD files in the directory\n"
            << "-t           PCD file types: PointXYZ | PointXYZRGB (Default)\n"
            << "\n\n";
}

static void saveLog(const std::string& pwd, const std::string& fileName, const std::string& content)
{
    std::string logFileName = pwd;
    logFileName.append("pcd-viewer.log");
    std::ofstream logFile;
    logFile.open(logFileName, std::ios::out | std::ios::app);
    if(logFile.is_open())
    {
        logFile << "[" << fileName << "]" << std::endl;
        logFile << "--------------------" << std::endl;
        logFile << content << std::endl;
        logFile.close();
    }
    else
    {
        std::cout << "[saveLog] >> Cloud not open log file: " << logFileName << std::endl;
    }
}

static void pointPickingEvent_Callback(const pcl::visualization::PointPickingEvent &event, void *args)
{
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *reinterpret_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer> *>(args);
    if (event.getPointIndex() == -1)
    return;
    viewer->removeText3D("pointCoordinates");
    pcl::PointXYZRGB selected_point;
    event.getPoint(selected_point.x, selected_point.y, selected_point.z);
    // std::cout << "Selected Point: [" << selected_point.x << ", " << selected_point.y << ", " << selected_point.z << "]" << std::endl;
    char point_coordinates[30];
    std::sprintf(point_coordinates, "%0.6f, %0.6f, %0.6f", selected_point.x, selected_point.y, selected_point.z);
    viewer->addText3D(point_coordinates, selected_point, 0.025, 1.0, 0.0, 0.0, "pointCoordinates");
}

static void keyBoardEvent_Callback(const pcl::visualization::KeyboardEvent &event, void *args)
{
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *reinterpret_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer> *>(args);
    // std::cout << event.getKeyCode() << std::endl;
    if(event.keyUp())
    {
        std::cout << event.getKeySym() << std::endl;
    }
}
// template<typename PointT>
// class Visualization
// {
//     typedef typename pcl::PointCloud<PointT> PointCloud;
//     typedef typename pcl::PointCloud<PointT>::Ptr PointCloudPtr;
// 
//     public:
//     Visualization(): m_cloud(new PointCloud),
//                      m_viewer(new pcl::visualization::PCLVisualizer("PCD Viewer"))
//     {}
//     ~Visualization() =default;
//     
//     private:
//     PointCloudPtr m_cloud;
//     // std::vector<PointT> m_vector;
//     boost::shared_ptr<pcl::visualization::PCLVisualizer> m_viewer;
//     std::string m_path;
// };

template<typename PointT> static bool visualization(const std::string& path)
{
    std::string pwd;
    std::string fileName;
    std::string::size_type pos = path.find_last_of('/');
    pwd = path.substr(0, pos+1);
    fileName = path.substr(pos+1);


    typedef typename pcl::PointCloud<PointT> PointCloud;
    typedef typename pcl::PointCloud<PointT>::Ptr PointCloudPtr;
    PointCloudPtr cloud(new PointCloud);
    try
    {
        if(pcl::io::loadPCDFile<PointT>(path, *cloud) == -1)
        {
            std::string logContent("[Visualization] >> Could not load the PCD file: ");
            logContent.append(path);
            saveLog(pwd, fileName, logContent);
            // std::cout << "[visualization] >> Could not load the PCD file: " << path << std::endl;
            return false;
        }
    }
    catch(const pcl::IOException& e)
    {
        std::string logContent("[Visualization] >> ");
        logContent.append(e.detailedMessage());
        saveLog(pwd, fileName, logContent);
        return false;
    }
    
    try
    {
        boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer(fileName));

        viewer->setBackgroundColor(255, 255, 255);
        viewer->addCoordinateSystem(1.0);
        viewer->initCameraParameters();
        viewer->addPointCloud<PointT>(cloud, fileName);
        viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, fileName);
        viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 1.0, fileName);
        viewer->registerPointPickingCallback(pointPickingEvent_Callback, reinterpret_cast<void *>(&viewer));
        viewer->registerKeyboardCallback(keyBoardEvent_Callback, reinterpret_cast<void *>(&viewer));

        viewer->spin();
    }
    catch(const pcl::UnhandledPointTypeException& e)
    {
        std::string logContent("[Visualization] >> ");
        logContent.append(e.detailedMessage());
        saveLog(pwd, fileName, logContent);
        return false;
    }
    return true;
}

int main (int argc, char** argv)
{
    std::string path;
    std::string type;
    if(pcl::console::find_argument(argc, argv, "-h") >= 0)
    {
        printUsage(argv[0]);
        return 0;
    }
    else if(pcl::console::parse_argument(argc, argv, "-d", path) != -1)
    {
        // std::cout << "[main] >> Visualize all the PCD files: ../data/000.pcd" << std::endl;
        // path = "../data/000.pcd";
        // if(pcl::console::parse_argument(argc, argv, "-t", type) != -1)
        // {
        //     if(type.find("RGB") == std::string::npos)
        //     {
        //         visualization<pcl::PointXYZ>(path);
        //     }
        //     else
        //     {
        //         visualization<pcl::PointXYZRGB>(path);
        //     }
        // }
        // else
        // {
        //     visualization<pcl::PointXYZ>(path);
        // }
    }
    else if(pcl::console::parse_argument(argc, argv, "-f", path) != -1)
    {
        // std::cout << "[main] >> Visualize the PCD files listed in the file: " << path << std::endl;
    }
    else if(pcl::console::parse_argument(argc, argv, "-p", path) != -1)
    {
        // std::cout << "[main] >> Visualize the PCD file in the path: " << path << std::endl;
        if(pcl::console::parse_argument(argc, argv, "-t", type) != -1)
        {
            if(type.find("RGB") == std::string::npos)
            {
                visualization<pcl::PointXYZ>(path);
            }
            else
            {
                visualization<pcl::PointXYZRGB>(path);
            }
        }
        else
        {
            visualization<pcl::PointXYZ>(path);
        }
    }
    else
    {
        // printUsage(argv[0]);
        // std::ofstream file;
        // file.open("/home/bee/GitRepositories/pcl-samples/data/test.txt", std::ios::out | std::ios::trunc);
        // file.write(argv[1], 100);
        // file.close();
        if(visualization<pcl::PointXYZRGB>(argv[1]) == false)
        {
            visualization<pcl::PointXYZ>(argv[1]);
        }
        return 0;
    }
    
//   pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
//   if ( pcl::io::loadPCDFile <pcl::PointXYZ> ("../data/region_growing_tutorial.pcd", *cloud) == -1)
//   {
//     std::cout << "Cloud reading failed." << std::endl;
//     return (-1);
//   }
// 
//   std::clock_t start, end;
// 
//   pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
//   pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
//   sor.setInputCloud(cloud);
//   sor.setMeanK(50);
//   sor.setStddevMulThresh(1.0);
//   start = std::clock();
//   sor.filter(*cloud_filtered);
//   end = std::clock();
//   std::cout << "Time used for statistical outlier removal is: " << (double)(end-start)/CLOCKS_PER_SEC << "s." << std::endl;
// 
//   pcl::search::Search<pcl::PointXYZ>::Ptr tree = boost::shared_ptr<pcl::search::Search<pcl::PointXYZ> > (new pcl::search::KdTree<pcl::PointXYZ>);
//   pcl::PointCloud <pcl::Normal>::Ptr normals (new pcl::PointCloud <pcl::Normal>);
//   pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normal_estimator;
//   normal_estimator.setSearchMethod (tree);
//   normal_estimator.setInputCloud (cloud_filtered);
//   normal_estimator.setKSearch (50);
//   start = std::clock();
//   normal_estimator.compute (*normals);
//   end = std::clock();
//   std::cout << "Time used for normal estimation is: " << (double)(end-start)/CLOCKS_PER_SEC << "s." << std::endl;
// 
//   pcl::IndicesPtr indices (new std::vector <int>);
//   pcl::PassThrough<pcl::PointXYZ> pass;
//   pass.setInputCloud (cloud_filtered);
//   pass.setFilterFieldName ("z");
//   pass.setFilterLimits (0.0, 1.0);
//   start = std::clock();
//   pass.filter (*indices);
//   end = std::clock();
//   std::cout << "Time used for pass through is: " << (double)(end-start)/CLOCKS_PER_SEC << "s." << std::endl;
// 
//   pcl::RegionGrowing<pcl::PointXYZ, pcl::Normal> reg;
//   reg.setMinClusterSize (50);
//   reg.setMaxClusterSize (1000000);
//   reg.setSearchMethod (tree);
//   reg.setNumberOfNeighbours (30);
//   reg.setInputCloud (cloud_filtered);
//   //reg.setIndices (indices);
//   reg.setInputNormals (normals);
//   reg.setSmoothnessThreshold (3.0 / 180.0 * M_PI);
//   reg.setCurvatureThreshold (1.0);
// 
//   std::vector <pcl::PointIndices> clusters;
//   start = std::clock();
//   reg.extract (clusters);
//   end = std::clock();
//   std::cout << "Time used for region growing is: " << (double)(end-start)/CLOCKS_PER_SEC << "s." << std::endl;
// 
// 
//   std::cout << "Number of clusters is equal to " << clusters.size () << std::endl;
//   std::cout << "First cluster has " << clusters[0].indices.size () << " points." << endl;
//   std::cout << "These are the indices of the points of the initial" <<
//     std::endl << "cloud that belong to the first cluster:" << std::endl;
//   // int counter = 0;
//   // while (counter < clusters[0].indices.size ())
//   // {
//   //   std::cout << clusters[0].indices[counter] << ", ";
//   //   counter++;
//   //   if (counter % 10 == 0)
//   //     std::cout << std::endl;
//   // }
//   // std::cout << std::endl;
// 
//   pcl::PointCloud <pcl::PointXYZRGB>::Ptr colored_cloud = reg.getColoredCloud ();
//   std::cout << *colored_cloud << std::endl;
// 
//   // pcl::visualization::CloudViewer viewer ("Cluster viewer");
//   // viewer.showCloud(colored_cloud);
//   // while (!viewer.wasStopped ())
//   // {
//   // }
// 
//   boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("Viewer"));
//   viewer->setBackgroundColor(0, 0, 0);
//   viewer->addCoordinateSystem(1.0);
//   viewer->initCameraParameters();
//   pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(colored_cloud);
//   viewer->addPointCloud<pcl::PointXYZRGB>(colored_cloud, rgb, "Cloud Segmented");
//   viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "Cloud Segmented");
//   viewer->spin();

  return (0);
}

