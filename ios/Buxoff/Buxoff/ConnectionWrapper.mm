#import "ConnectionWrapper.h"
#import "Storage.h"

@interface ConnectionWrapper()
@property Buxoff::Connection *impl;
@end

@implementation ConnectionWrapper

+ (instancetype)sharedConnection
{
    static ConnectionWrapper *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[self alloc] init];
    });
    return instance;
}

- (instancetype)init
{
    NSLog(@"create connection wrapper");
    self = [super init];
    
    if (self != nil) {
        NSString *path = [self getPath];
        _impl = new Buxoff::Connection{[path UTF8String]};
    }
    
    return self;
}

- (void)dealloc
{
    NSLog(@"close connection");
    delete _impl;
}

- (NSString *)getPath
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0]; // Get documents folder
    NSString *dataPath = [documentsDirectory stringByAppendingPathComponent:@"Buxoff.db"];
    
    NSLog(@"path %@", dataPath);

    return dataPath;
}

@end
