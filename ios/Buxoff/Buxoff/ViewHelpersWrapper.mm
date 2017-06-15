//
//  ViewHelpers.m
//  Buxoff
//
//  Created by Viacheslav Iutin on 19/05/2017.
//  Copyright © 2017 Viacheslav Iutin. All rights reserved.
//

#import "ViewHelpersWrapper.h"
#import "ViewHelpers.h"
#import "ConnectionWrapper.h"
#import "ConnectionWrapper+Impl.h"
#import "Record.h"


@implementation ViewHelpersWrapper

+ (BOOL)enableAdd:(NSString*)amount account:(NSString*)account
{
    return Buxoff::view_enable_add(amount.UTF8String, account.UTF8String);
}

+ (BOOL)enablePush:(int)record_count amount:(NSString*)amount account:(NSString *)account email:(NSString*)email
{
    return Buxoff::view_enable_push(record_count, amount.UTF8String, account.UTF8String, email.UTF8String);
}

+ (NSString*)tags:(NSString*)desc
{
    auto tags = tags_for_description([ConnectionWrapper sharedConnection].impl, desc.UTF8String);
    return [NSString stringWithUTF8String:tags.c_str()];
}

@end
