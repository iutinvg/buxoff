//
//  UserDefaultsWrapper.h
//  Buxoff
//
//  Created by Viacheslav Iutin on 19/05/2017.
//  Copyright © 2017 Viacheslav Iutin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface UserDefaultsWrapper : NSObject
+ (void)put:(NSString*)key value:(NSString*)value;
+ (NSString*)get:(NSString*)key def:(NSString*)def;
@end
