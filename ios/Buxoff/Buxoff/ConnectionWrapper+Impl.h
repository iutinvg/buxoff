#import "Storage.h"

@interface ConnectionWrapper(Internal)
@property (nonatomic, readonly) Buxoff::Connection *impl;
@end
