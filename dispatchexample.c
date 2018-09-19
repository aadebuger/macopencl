#include <stdio.h>
 
// Include OpenCL/opencl.h to include everything you need for OpenCL
// development on OS X v10.7 or later.
#include <OpenCL/opencl.h>
 
// In this example, mykernel.cl.h is the header file that contains
// the kernel block declaration.  The name of this header file would
// be different if the name of the file containing the kernel source
// were different.
// This header file is generated by Xcode.
#include "mykernel.cl.h"
 
static void print_device_info(cl_device_id device) {
    char name[128];
    char vendor[128];
 
    clGetDeviceInfo(device, CL_DEVICE_NAME, 128, name, NULL);
    clGetDeviceInfo(device, CL_DEVICE_VENDOR, 128, vendor, NULL);
    fprintf(stdout, "%s : %s\n", vendor, name);
}
 
// Demonstrates how to get the global OpenCL context, and how to ask that
// context about the devices it contains.  It also shows how
// to create a dispatch queue by asking for a device type (CPU or GPU) and
// by specifying the queue's OpenCL device directly.
 
static void hello_world_sample1 ()
{
    int i;
 
    // Ask for the global OpenCL context:
    // Note: If you will not be enqueing to a specific device, you do not need
    // to retrieve the context.
 
    cl_context context = gcl_get_context();
 
    // Query this context to see what kinds of devices are available.
 
    size_t length;
    cl_device_id devices[8];
    clGetContextInfo(
         context, CL_CONTEXT_DEVICES, sizeof(devices), devices, &length);
 
    // Walk over these devices, printing out some basic information.  You could
    // query any of the information available about the device here.
 
    fprintf(stdout, "The following devices are available for use:\n");
    int num_devices = (int)(length / sizeof(cl_device_id));
    for (i = 0; i < num_devices; i++) {
        print_device_info(devices[i]);
    }
 
    // To do any work, you need to create a dispatch queue associated
    // with some OpenCL device.  You can either let the system give you
    // a GPU—perhaps the only GPU—or the CPU device.  Or, you can
    // create a dispatch queue with a cl_device_id you specify.  This
    // device id comes from the OpenCL context, as above.  Below are three
    // examples.
 
    // 1. Ask for a GPU-based dispatch queue; notice that here we do not provide
    // a device id.  Instead, we let the system tell us the most capable GPU.
 
    dispatch_queue_t gpu_queue =
       gcl_create_dispatch_queue(CL_DEVICE_TYPE_GPU, NULL);
 
    // Get the device from the queue, so we can ask OpenCL questions about it.
    // Note that we check to make sure there WAS an OpenCL-capable GPU in the
    // system by checking against a NULL return value.
 
    if (gpu_queue != NULL) {
 
        cl_device_id gpu_device =
          gcl_get_device_id_with_dispatch_queue(gpu_queue);
        fprintf(stdout, "\nAsking for CL_DEVICE_TYPE_GPU gives us:\n");
        print_device_info(gpu_device);
 
    } else {
        fprintf(stdout, "\nYour system does not contain an OpenCL-compatible "
                "GPU\n.");
    }
 
    // 2. Try the same thing for CL_DEVICE_TYPE_CPU.  All Mac
    // systems have a CPU OpenCL device, so you don't have to
    // check for NULL, as you have to do in the case of a GPU.
 
    dispatch_queue_t cpu_queue =
        gcl_create_dispatch_queue(CL_DEVICE_TYPE_CPU, NULL);
    cl_device_id cpu_device = gcl_get_device_id_with_dispatch_queue(cpu_queue);
    fprintf(stdout, "\nAsking for CL_DEVICE_TYPE_CPU gives us:\n");
    print_device_info(cpu_device);
 
    // 3. Or perhaps you are in a situation where you want a specific device
    // from the list of devices you found on the context.
    // Notice the difference here:
    // Pass CL_DEVICE_TYPE_USE_ID and a device_id. This example just uses the
    // first device on the context from above, whatever that might be.
 
    dispatch_queue_t custom_queue =
        gcl_create_dispatch_queue(CL_DEVICE_TYPE_USE_ID, devices[0]);
    cl_device_id custom_device =
        gcl_get_device_id_with_dispatch_queue(custom_queue);
    fprintf(stdout,
       "\nAsking for CL_DEVICE_TYPE_USE_ID and our own device gives us:\n");
    print_device_info(custom_device);
 
    // Now you can use any of these 3 dispatch queues to run some kernels.
    …                                              // Run your kernels here.
 
    // Use the GCD API to free your queues.
 
    dispatch_release(custom_queue);
    dispatch_release(cpu_queue);
 
    if (gpu_queue != NULL) dispatch_release(gpu_queue);
}
 

