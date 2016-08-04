//
//  StorageWrapper.h
//  Buxoff
//
//  Created by Viacheslav Iutin on 03/08/2016.
//  Copyright © 2016 Viacheslav Iutin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface StorageWrapper : NSObject

- (instancetype)init;
- (NSInteger)getCount;
- (NSString*)addTestRecord;

@end
