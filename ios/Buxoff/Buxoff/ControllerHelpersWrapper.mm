//
//  ControllerHelpers.m
//  Buxoff
//
//  Created by Viacheslav Iutin on 20/05/2017.
//  Copyright © 2017 Viacheslav Iutin. All rights reserved.
//

#import "ConnectionWrapper.h"
#import "ConnectionWrapper+Impl.h"
#import "ControllerHelpersWrapper.h"
#import "ControllerHelpers.h"
#import "Record.h"

@implementation ControllerHelpersWrapper

+ (void)add:(NSString*)amount desc:(NSString*)desc tag:(NSString*)tag account:(NSString*)account
{
    Buxoff::Record r{
        [amount UTF8String],
        [desc UTF8String],
        {[tag UTF8String]},
        [account UTF8String]};
    controller_add([ConnectionWrapper sharedConnection].impl, r);
}

+ (NSString*)push:(NSString*)amount desc:(NSString*)desc tag:(NSString*)tag account:(NSString*)account
{
    Buxoff::Record r{
        [amount UTF8String],
        [desc UTF8String],
        {[tag UTF8String]},
        [account UTF8String]};
    auto body = controller_push([ConnectionWrapper sharedConnection].impl, r);
    return [NSString stringWithUTF8String:body.c_str()];
}

@end
