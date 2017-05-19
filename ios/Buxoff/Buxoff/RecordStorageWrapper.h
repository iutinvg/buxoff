#import <Foundation/Foundation.h>

@interface RecordStorageWrapper : NSObject

+ (int)count;
+ (void)add:(NSString*)amount desc:(NSString*)desc tag:(NSString*)tag account:(NSString*)account;

@end
