//
//  UserDefaultsWrapper.m
//  Buxoff
//
//  Created by Viacheslav Iutin on 19/05/2017.
//  Copyright © 2017 Viacheslav Iutin. All rights reserved.
//

#import "UserDefaultsWrapper.h"
#import "ConnectionWrapper.h"
#import "ConnectionWrapper+Impl.h"
#import "UserDefaults.h"

@implementation UserDefaultsWrapper
+ (void)put:(NSString*)key value:(NSString*)value
{
    Buxoff::UserDefaults ud{[ConnectionWrapper sharedConnection].impl};
    ud.put(key.UTF8String, value.UTF8String);
}

+ (NSString*)get:(NSString*)key def:(NSString*)def
{
    Buxoff::UserDefaults ud{[ConnectionWrapper sharedConnection].impl};
    return [NSString stringWithUTF8String:ud.get(key.UTF8String, def.UTF8String).c_str()];
}
@end
