//
//  EmailWrapper.m
//  Buxoff
//
//  Created by Viacheslav Iutin on 20/05/2017.
//  Copyright © 2017 Viacheslav Iutin. All rights reserved.
//

#import "EmailWrapper.h"
#import "Email.h"
#import "ControllerHelpers.h"


@implementation EmailWrapper
+ (NSString*)subject
{
    return [NSString stringWithUTF8String:Buxoff::Email::subject().c_str()];
}
@end
