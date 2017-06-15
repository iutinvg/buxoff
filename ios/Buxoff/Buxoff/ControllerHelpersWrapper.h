//
//  ControllerHelpers.h
//  Buxoff
//
//  Created by Viacheslav Iutin on 20/05/2017.
//  Copyright © 2017 Viacheslav Iutin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ControllerHelpersWrapper : NSObject
+ (void)add:(NSString*)amount desc:(NSString*)desc tag:(NSString*)tag account:(NSString*)account;
+ (NSString*)push:(NSString*)amount desc:(NSString*)desc tag:(NSString*)tag account:(NSString*)account;
+ (void)clear_records;
@end
