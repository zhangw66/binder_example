#include <binder/IServiceManager.h>
#include <binder/IBinder.h>
#include <binder/Parcel.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <android/log.h>
using namespace android;
#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "testService"

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "ProjectName", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "ProjectName", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , "ProjectName", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN  , "ProjectName", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "ProjectName", __VA_ARGS__)

class MyService : public BBinder
{
public:
    MyService()
    {
        mydescriptor = String16("media.hello");
        n=0;    
    }
    virtual ~MyService() {}
    //This function is used when call Parcel::checkInterface(IBinder*)
    virtual const String16& getInterfaceDescriptor() const
    {
        LOGE("this is enter ==========getInterfaceDescriptor");
        return mydescriptor;
    }
protected:
    void show()
    {
        LOGE("this is for test show!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        LOGE("this is for test show!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        LOGE("this is for test show!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        LOGE("this is for test show!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        LOGE("this is for test show!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
    virtual status_t onTransact( uint32_t code,const Parcel& data,Parcel* reply,uint32_t flags = 0)
    {
        LOGD("enter MyService onTransact and the code is %d",code);
        /*
        if (data.checkInterface(this))
        LOGD("checkInterface OK");
        else
        {
        LOGW("checkInterface failed");
        return BBinder::onTransact(code, data, reply, flags);
        }
        */
        switch (code)
        {
            case 1:
                LOGD("MyService interface 1");
                break;
            case 2:
                LOGD("MyService interface 2");
                cb = data.readStrongBinder();
                break;
            case 3:
            {
                LOGD("MyService interface 3, exit");
                //No unregister service routine?
                //It should return to client first and then call exit in another place.
                exit(0);
                break;
            }
            case 4:
            {//call cb
                LOGD("MyService interface 4 before if================");
                cb = data.readStrongBinder();
                if (cb != NULL)
                {
                    LOGD("MyService interface 4");
                    Parcel in, out;
                    in.writeInterfaceToken(String16("android.os.ISetupCallback"));
                    in.writeInt32(n++); //向客户端发送数据
                    
                    in.writeCString("This is a string !");
                    cb->transact(2, in, &out, 0);
                    show();
                }
                break;
            }
            default:
                return BBinder::onTransact(code, data, reply, flags);
        }
        return 0;
    }
private:
    String16 mydescriptor;
    sp<IBinder> cb;
    int n;
};
int main()
{
    sp<IServiceManager> sm = defaultServiceManager(); //获取ServiceManager服务代理
    status_t ret;
    //register MyService to ServiceManager
    MyService* srv = new MyService();
    ret = sm->addService(String16("media.hello"), srv); // 注册服务
    LOGD("addservice media.hello return %d", ret);
    //call binder thread pool to start
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool(true); //参数默认也是true，进入服务的循环监听状态
    return 0;
}