//
//  ViewController.m
//  Buxoff
//
//  Created by Viacheslav Iutin on 30/07/2016.
//  Copyright © 2016 Viacheslav Iutin. All rights reserved.
//

#import "StorageWrapper.h"
#import "Storage.h"

@interface StorageWrapper ()

@property (nonatomic, readonly) Buxoff::Storage* internal;

@end

@implementation StorageWrapper : NSObject

- (instancetype)init
{
    self = [super init];
    
    if (self != nil) {
        NSString *path = [self getPath];
        _internal = new Buxoff::Storage{std::string{[path UTF8String]}};
    }
    
    return self;
}

- (void)dealloc
{
    delete _internal;
}

- (NSString *)getPath
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0]; // Get documents folder
    NSString *dataPath = [documentsDirectory stringByAppendingPathComponent:@"Buxfer.db"];
    
    NSLog(@"path %@", dataPath);

    return dataPath;
}

- (NSInteger)getCount
{
    return _internal->get_records_count();
}

- (NSString*)addTestRecord
{
    Buxoff::Record r = Buxoff::Record([@"12.45" UTF8String],
                                      [@"desc" UTF8String],
                                      {[@"t1" UTF8String], [@"t2" UTF8String]}, [@"cash" UTF8String]);
    NSString *res = [NSString stringWithUTF8String:r.get_json_string().c_str()];
    NSLog(@"res is %@", res);
    // Do any additional setup after loading the view, typically from a nib.
    auto uid = _internal->put(r);
    return [NSString stringWithUTF8String:uid.c_str()];
}

@end
