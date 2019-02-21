#include <binder/IServiceManager.h>
#include <binder/IBinder.h>
#include <binder/Parcel.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <private/binder/binder_module.h>
#include <android/log.h>

using namespace android;
#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "testCallback"

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "ProjectName", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "ProjectName", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , "ProjectName", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN  , "ProjectName", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "ProjectName", __VA_ARGS__)

class MySetupCallback : public BBinder
{
public:
    MySetupCallback()
    {
        mydescriptor = String16("android.os.ISetupCallback");
    }
    virtual ~MySetupCallback() {}
    virtual const String16& getInterfaceDescriptor() const
    {
        return mydescriptor;
    }
protected:
    virtual status_t onTransact( uint32_t code,    const Parcel& data,    Parcel* reply,    uint32_t flags = 0)
    {
        LOGD("enter MySetupCallback onTransact, code=%u", code);
        if (data.checkInterface(this)) //检查 mydescriptor 类描述字符串
            LOGD("checkInterface OK");
        else
        {
            LOGW("checkInterface failed");
            return -1;
        }
        switch (code) //code为服务器发送的code，根据code实现不同的函数
        {
            case 1:
                LOGD("From Server code = %u", code);
                LOGD("From Server code = %u", code);
                break;
            case 2:
            {
                LOGD("From Server code = %u", code);  
                LOGD("Frome server data = %d", data.readInt32()); //从服务端接收数据
                LOGD("Frome server string = %s", data.readCString());
                
                break;
            }
            default:
            break;
        }
        return 0;
    }
private:
    String16 mydescriptor;
};

int main()
{
    sp<IServiceManager> sm = defaultServiceManager();   //获取ServiceManager服务代理
    sp<IBinder> b = sm->getService(String16("media.hello")); //查询服务
    if (b == NULL)
    {
        LOGW("Can't find binder service \"media.hello\"");
        return -1;
    }
    Parcel in1,out1; 
    MySetupCallback *cb = new MySetupCallback();
    in1.writeStrongBinder(sp<IBinder>(cb));
    int ret = b->transact(4, in1, &out1, 0); //TRANSACTION_registerSetup = 4
    LOGD("transact(4) return %d", ret);
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool(); //参数默认也是true，进入服务的循环监听状态
    return 0;
}