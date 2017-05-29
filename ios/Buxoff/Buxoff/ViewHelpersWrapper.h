//
//  ViewHelpers.h
//  Buxoff
//
//  Created by Viacheslav Iutin on 19/05/2017.
//  Copyright © 2017 Viacheslav Iutin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ViewHelpersWrapper : NSObject
+ (BOOL)enableAdd:(NSString*)amount account:(NSString*)account;
+ (BOOL)enablePush:(int)record_count amount:(NSString*)amount account:(NSString *)account email:(NSString*)email;
+ (NSString*)tags:(NSString*)desc;
@end
