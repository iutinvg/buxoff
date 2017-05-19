//
//  RecordWrapper.m
//  Buxoff
//
//  Created by Viacheslav Iutin on 15/05/2017.
//  Copyright © 2017 Viacheslav Iutin. All rights reserved.
//

#import "RecordStorageWrapper.h"
#import "ConnectionWrapper.h"
#import "ConnectionWrapper+Impl.h"
#import "Record.h"
#import "ControllerHelpers.h"


@implementation RecordStorageWrapper

+ (int)count
{
    Buxoff::RecordStorage rs{[ConnectionWrapper sharedConnection].impl};
    return rs.count();
}

+ (void)add:(NSString*)amount desc:(NSString*)desc tag:(NSString*)tag account:(NSString*)account
{
    Buxoff::Record r{
        [amount UTF8String],
        [desc UTF8String],
        {[tag UTF8String]},
        [account UTF8String]};
    controller_add([ConnectionWrapper sharedConnection].impl, r);
}

@end
