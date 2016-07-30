//
//  ViewController.m
//  Buxoff
//
//  Created by Viacheslav Iutin on 30/07/2016.
//  Copyright © 2016 Viacheslav Iutin. All rights reserved.
//

#import "ViewController.h"
#import "Record.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    Buxoff::Record r = Buxoff::Record([@"12.45" UTF8String],
                                      [@"desc" UTF8String],
                                      {[@"t1" UTF8String], [@"t2" UTF8String]}, [@"cash" UTF8String]);
    NSString *res = [NSString stringWithUTF8String:r.get_json_string().c_str()];
    NSLog(@"res is %@", res);
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
