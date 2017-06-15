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

@end
