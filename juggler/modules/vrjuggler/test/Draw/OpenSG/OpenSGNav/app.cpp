#include <OpenSGNav.h>

#include <vrj/Kernel/Kernel.h>

#ifndef WIN32
#include <sched.h>
#endif

int main(int argc, char* argv[])
{
    std::cout<<"osgInit begin\n";
    OSG::osgInit(argc,argv);
    OpenSGNav::OSG_MAIN_ASPECT_ID = OSG::Thread::getAspect();
    std::cout<<"osgInit end\n";
    vrj::Kernel* kernel = vrj::Kernel::instance();
    std::cout<<"vjKernel instantiated\n";
    OpenSGNav* application = new OpenSGNav(kernel);

    if (argc <= 2)
    {
        std::cout<<"\n"<<std::flush;
        std::cout<<"\n"<<std::flush;

        std::cout<<"Usage: "<<argv[0]<<" modelname vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n"<<std::flush;

        std::cout<<"\n"<<std::flush;
        std::cout<<"\n"<<std::flush;
        exit(1);
    }

    application->setModelFileName(std::string(argv[1]));

    std::cout<<"inp man: "<<kernel->getInputManager()<<"\n";
    // Load any config files specified on the command line
    std::cout<<"loading config files\n";
    for(int i=2;i<argc;i++)
        kernel->loadConfigFile(argv[i]);
    std::cout<<"loading config files ended\n";
    kernel->start();

    kernel->setApplication(application);

    printf("entering infinite while\n");
    while(1)
    {
        usleep(250000);
    }
    return (1);
}
